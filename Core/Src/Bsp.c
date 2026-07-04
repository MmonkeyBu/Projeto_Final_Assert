/**
 * @file    Bsp.c
 * @addtogroup BSP
 * @{
 * @brief   Implementacao da camada de abstracao de hardware (BSP).
 *
 * Centraliza o acesso direto aos perifericos: ADC1, TIM2 (amostragem),
 * TIM6 (debounce), TIM3/TIM4/TIM12 (PWM) e USART3. Os callbacks da HAL
 * sao implementados neste arquivo para desacoplar os modulos de logica
 * do hardware fisico.
 */

/* Includes ------------------------------------------------------------------*/
#include "Bsp.h"
#include <string.h>

/* Defines Locais ------------------------------------------------------------*/

/// Tamanho do buffer circular de recepcao da UART (unidade: bytes, potencia de 2)
#define RX_BUFFER_SIZE  16U

/* Constantes ----------------------------------------------------------------*/

/* Variaveis Externas --------------------------------------------------------*/
// Handles dos perifericos gerados pelo STM32CubeMX
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim12;
extern UART_HandleTypeDef huart3;

/* Estruturas de Dados Locais ------------------------------------------------*/

/* Variaveis Locais ----------------------------------------------------------*/

// Flag setada pelo ISR do TIM2 a cada 5 ms para sinalizar nova amostragem
static volatile uint8_t g_sampling_pending = 0U;

// Flag setada pelo ISR do TIM6 ao fim do debounce para sinalizar pressao valida
static volatile uint8_t g_debounce_pending = 0U;

// Buffer circular para recepcao de bytes via UART por interrupcao
static volatile uint8_t g_rx_buffer[RX_BUFFER_SIZE];

// Indice de escrita no buffer circular (preenchido pelo ISR da UART)
static volatile uint8_t g_rx_head = 0U;

// Indice de leitura no buffer circular (consumido pelo loop principal)
static volatile uint8_t g_rx_tail = 0U;

// Registrador de um byte para o HAL_UART_Receive_IT
static uint8_t g_current_rx_byte = 0U;

/* Prototipos Locais ---------------------------------------------------------*/

/* Funcoes Publicas ----------------------------------------------------------*/

/**
 * @brief   Inicializa os perifericos da BSP.
 * @details Inicia os canais de PWM com duty zero e habilita a recepcao
 *          de dados pela UART via interrupcao.
 * @param   Nenhum.
 * @retval  Nenhum.
 */
void Bsp_Init(void)
{
    Bsp_StartPwm();
    // Garante que todos os LEDs iniciam apagados
    Bsp_SetPwmDuty(1U, 0U);
    Bsp_SetPwmDuty(2U, 0U);
    Bsp_SetPwmDuty(3U, 0U);
    // Arma a recepcao do primeiro byte pela UART
    HAL_UART_Receive_IT(&huart3, &g_current_rx_byte, 1U);
}

/**
 * @brief   Inicia o timer de amostragem periodica do ADC.
 * @details Utiliza o TIM2 configurado para disparar a cada 5 ms.
 * @param   Nenhum.
 * @retval  Nenhum.
 */
void Bsp_StartSamplingTimer(void)
{
    HAL_TIM_Base_Start_IT(&htim2);
}

/**
 * @brief   Inicia o timer de debounce do botao.
 * @details Utiliza o TIM6 configurado para um periodo de debounce de 20 ms.
 * @param   Nenhum.
 * @retval  Nenhum.
 */
void Bsp_StartDebounceTimer(void)
{
    HAL_TIM_Base_Start_IT(&htim6);
}

/**
 * @brief   Para o timer de debounce.
 * @details Desabilita o TIM6 e sua interrupcao apos o fim do debounce.
 * @param   Nenhum.
 * @retval  Nenhum.
 */
void Bsp_StopDebounceTimer(void)
{
    HAL_TIM_Base_Stop_IT(&htim6);
}

/**
 * @brief   Inicia a geracao de sinal PWM nos tres canais dos LEDs.
 * @details Habilita TIM3-CH3 (LED1), TIM4-CH2 (LED2) e TIM12-CH1 (LED3).
 * @param   Nenhum.
 * @retval  Nenhum.
 */
void Bsp_StartPwm(void)
{
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
}

/**
 * @brief   Ajusta o duty cycle de um canal de PWM.
 * @details O valor de duty e saturado em BSP_PWM_MAX_VALUE caso ultrapasse
 *          o limite. O mapeamento de canais e: 1->TIM3, 2->TIM4, 3->TIM12.
 * @param   channel Canal do LED a ser ajustado (faixa valida: 1, 2 ou 3).
 * @param   duty    Valor do registrador de comparacao (faixa: 0 a BSP_PWM_MAX_VALUE).
 * @retval  Nenhum.
 */
void Bsp_SetPwmDuty(uint8_t channel, uint32_t duty)
{
    uint32_t ccr = duty;

    // Satura o duty para nao ultrapassar o periodo do timer
    if (ccr > BSP_PWM_MAX_VALUE) {
        ccr = BSP_PWM_MAX_VALUE;
    }

    if (channel == 1U) {
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, ccr);
    } else if (channel == 2U) {
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, ccr);
    } else if (channel == 3U) {
        __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, ccr);
    }
}

/**
 * @brief   Transmite uma mensagem de texto pela USART3.
 * @details Operacao bloqueante com timeout infinito.
 * @param   msg Ponteiro para o buffer de caracteres a ser transmitido.
 * @param   len Numero de bytes a transmitir (unidade: bytes).
 * @retval  Nenhum.
 */
void Bsp_Print(const char *msg, uint16_t len)
{
    // Transmite apenas se houver conteudo valido
    if (len > 0U) {
        HAL_UART_Transmit(&huart3, (uint8_t *)msg, len, HAL_MAX_DELAY);
    }
}

/**
 * @brief   Realiza a leitura do ADC1 por polling disparado por software.
 * @details Inicia a conversao, aguarda o resultado por ate 10 ms e
 *          retorna o valor bruto de 12 bits.
 * @param   Nenhum.
 * @retval  Valor bruto lido do ADC (faixa: 0 a 4095, adimensional).
 */
uint32_t Bsp_ReadAdc(void)
{
    uint32_t adc_value = 0U;

    HAL_ADC_Start(&hadc1);
    // Aguarda a conversao por no maximo 10 ms
    if (HAL_ADC_PollForConversion(&hadc1, 10U) == HAL_OK) {
        adc_value = HAL_ADC_GetValue(&hadc1);
    }
    HAL_ADC_Stop(&hadc1);

    return adc_value;
}

/**
 * @brief   Consulta se ha uma amostragem do ADC pendente de processamento.
 * @param   Nenhum.
 * @retval  1 se ha amostragem pendente, 0 caso contrario.
 */
uint8_t Bsp_IsSamplingPending(void)
{
    return g_sampling_pending;
}

/**
 * @brief   Limpa a flag de amostragem pendente.
 * @param   Nenhum.
 * @retval  Nenhum.
 */
void Bsp_ClearSamplingPending(void)
{
    g_sampling_pending = 0U;
}

/**
 * @brief   Consulta se o debounce do botao foi concluido.
 * @param   Nenhum.
 * @retval  1 se o debounce concluiu, 0 caso contrario.
 */
uint8_t Bsp_IsDebouncePending(void)
{
    return g_debounce_pending;
}

/**
 * @brief   Limpa a flag de debounce concluido.
 * @param   Nenhum.
 * @retval  Nenhum.
 */
void Bsp_ClearDebouncePending(void)
{
    g_debounce_pending = 0U;
}

/**
 * @brief   Le um byte do buffer circular de recepcao da UART.
 * @details Funcao nao-bloqueante. Retorna imediatamente se o buffer estiver vazio.
 * @param   out_byte Ponteiro para a variavel onde o byte lido sera armazenado.
 * @retval  1 se um byte foi lido com sucesso, 0 se o buffer estava vazio.
 */
uint8_t Bsp_GetRxByte(uint8_t *out_byte)
{
    // Verifica se ha dados no buffer circular comparando head e tail
    if (g_rx_head != g_rx_tail) {
        *out_byte = g_rx_buffer[g_rx_tail];
        // Avanca o ponteiro de leitura de forma circular
        g_rx_tail = (g_rx_tail + 1U) % RX_BUFFER_SIZE;
        return 1U;
    }
    return 0U;
}

/* Funcoes Locais / Callbacks da HAL -----------------------------------------*/

/**
 * @brief   Callback de estouro de periodo dos timers (HAL override).
 * @details Chamado automaticamente pelo IRQ do TIM2 (amostragem) e TIM6 (debounce).
 *          Seta as flags correspondentes para processamento no loop principal.
 * @param   htim Ponteiro para o handle do timer que gerou a interrupcao.
 * @retval  Nenhum.
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) {
        // Sinaliza nova amostragem a cada 5 ms
        g_sampling_pending = 1U;
    } else if (htim->Instance == TIM6) {
        // Fim do periodo de debounce: para o timer e sinaliza evento valido
        Bsp_StopDebounceTimer();
        g_debounce_pending = 1U;
    }
}

/**
 * @brief   Callback de interrupcao externa de GPIO (HAL override).
 * @details Chamado automaticamente pelo IRQ EXTI ao detectar borda no botao.
 *          Inicia o timer de debounce para validar o pressionamento.
 * @param   GPIO_Pin Mascara do pino que gerou a interrupcao.
 * @retval  Nenhum.
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == USER_Btn_Pin) {
        // Inicia a contagem de debounce ao detectar borda no botao
        Bsp_StartDebounceTimer();
    }
}

/**
 * @brief   Callback de recepcao completa da UART (HAL override).
 * @details Chamado automaticamente pelo IRQ da USART3 apos cada byte recebido.
 *          Armazena o byte no buffer circular e rearma a interrupcao para o
 *          proximo byte. Descarta o byte se o buffer estiver cheio.
 * @param   huart Ponteiro para o handle da UART que gerou a interrupcao.
 * @retval  Nenhum.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3) {
        uint8_t next_head = (g_rx_head + 1U) % RX_BUFFER_SIZE;
        // Armazena apenas se houver espaco no buffer (protege contra overflow)
        if (next_head != g_rx_tail) {
            g_rx_buffer[g_rx_head] = g_current_rx_byte;
            g_rx_head = next_head;
        }
        // Rearma a interrupcao para receber o proximo byte
        HAL_UART_Receive_IT(&huart3, &g_current_rx_byte, 1U);
    }
}

/** @} */
