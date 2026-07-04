/**
 * @file    Bsp.c
 * @brief   Implementação do acesso ao hardware (ADC, PWM, timers, EXTI, UART).
 */

/* Includes ------------------------------------------------------------------*/
#include "Bsp.h"
#include <string.h>

/* Defines Locais ------------------------------------------------------------*/
#define RX_BUFFER_SIZE  16U

/* Constantes ----------------------------------------------------------------*/

/* Variáveis Externas --------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim12;
extern UART_HandleTypeDef huart3;

/* Estruturas de Dados Locais ------------------------------------------------*/

/* Variáveis Locais ----------------------------------------------------------*/
static volatile uint8_t g_sampling_pending = 0U;
static volatile uint8_t g_debounce_pending = 0U;

static volatile uint8_t g_rx_buffer[RX_BUFFER_SIZE];
static volatile uint8_t g_rx_head = 0U;
static volatile uint8_t g_rx_tail = 0U;
static uint8_t g_current_rx_byte = 0U;

/* Protótipos Locais ---------------------------------------------------------*/

/* Funções Públicas ----------------------------------------------------------*/

/**
 * @brief Inicializa os periféricos da BSP.
 * @param Nenhum.
 * @retval Nenhum.
 */
void Bsp_Init(void)
{
    Bsp_StartPwm();
    Bsp_SetPwmDuty(1U, 0U);
    Bsp_SetPwmDuty(2U, 0U);
    Bsp_SetPwmDuty(3U, 0U);
    HAL_UART_Receive_IT(&huart3, &g_current_rx_byte, 1U);
}

/**
 * @brief Inicia o timer de amostragem (5 ms).
 * @param Nenhum.
 * @retval Nenhum.
 */
void Bsp_StartSamplingTimer(void)
{
    HAL_TIM_Base_Start_IT(&htim2);
}

/**
 * @brief Inicia o timer de debounce para o botão.
 * @param Nenhum.
 * @retval Nenhum.
 */
void Bsp_StartDebounceTimer(void)
{
    HAL_TIM_Base_Start_IT(&htim6);
}

/**
 * @brief Para o timer de debounce.
 * @param Nenhum.
 * @retval Nenhum.
 */
void Bsp_StopDebounceTimer(void)
{
    HAL_TIM_Base_Stop_IT(&htim6);
}

/**
 * @brief Inicia a geração de PWM para os LEDs.
 * @param Nenhum.
 * @retval Nenhum.
 */
void Bsp_StartPwm(void)
{
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
}

/**
 * @brief Ajusta o duty cycle de um canal PWM.
 * @param channel: 1, 2 ou 3 representando o LED correspondente.
 * @param duty: Valor do duty cycle (0 a BSP_PWM_MAX_VALUE).
 * @retval Nenhum.
 */
void Bsp_SetPwmDuty(uint8_t channel, uint32_t duty)
{
    uint32_t ccr = duty;
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
 * @brief Transmite uma string formatada pela USART3.
 * @param msg: Ponteiro para a string a ser enviada.
 * @param len: Tamanho da string.
 * @retval Nenhum.
 */
void Bsp_Print(const char *msg, uint16_t len)
{
    if (len > 0U) {
        HAL_UART_Transmit(&huart3, (uint8_t *)msg, len, HAL_MAX_DELAY);
    }
}

/**
 * @brief Realiza a leitura do ADC1 via polling.
 * @param Nenhum.
 * @retval Valor lido do ADC (0 a 4095).
 */
uint32_t Bsp_ReadAdc(void)
{
    uint32_t adc_value = 0U;

    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 10U) == HAL_OK) {
        adc_value = HAL_ADC_GetValue(&hadc1);
    }
    HAL_ADC_Stop(&hadc1);

    return adc_value;
}

/**
 * @brief Verifica a flag de amostragem pendente.
 * @param Nenhum.
 * @retval 1 se houver amostragem pendente, 0 caso contrário.
 */
uint8_t Bsp_IsSamplingPending(void)
{
    return g_sampling_pending;
}

/**
 * @brief Limpa a flag de amostragem pendente.
 * @param Nenhum.
 * @retval Nenhum.
 */
void Bsp_ClearSamplingPending(void)
{
    g_sampling_pending = 0U;
}

/**
 * @brief Verifica a flag de debounce concluído.
 * @param Nenhum.
 * @retval 1 se o debounce concluiu, 0 caso contrário.
 */
uint8_t Bsp_IsDebouncePending(void)
{
    return g_debounce_pending;
}

/**
 * @brief Limpa a flag de debounce concluído.
 * @param Nenhum.
 * @retval Nenhum.
 */
void Bsp_ClearDebouncePending(void)
{
    g_debounce_pending = 0U;
}

/**
 * @brief Lê o byte recebido pela UART, se disponível.
 * @param out_byte: Ponteiro para armazenar o byte.
 * @retval 1 se um byte foi lido, 0 se não havia byte.
 */
uint8_t Bsp_GetRxByte(uint8_t *out_byte)
{
    if (g_rx_head != g_rx_tail) {
        *out_byte = g_rx_buffer[g_rx_tail];
        g_rx_tail = (g_rx_tail + 1U) % RX_BUFFER_SIZE;
        return 1U;
    }
    return 0U;
}

/* Funções Locais / Callbacks da HAL -----------------------------------------*/

/**
 * @brief Callback de interrupção de timers (Amostragem e Debounce).
 * @param htim: Ponteiro para o handle do timer.
 * @retval Nenhum.
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) {
        g_sampling_pending = 1U;
    } else if (htim->Instance == TIM6) {
        Bsp_StopDebounceTimer();
        g_debounce_pending = 1U;
    }
}

/**
 * @brief Callback de interrupção externa (Botão).
 * @param GPIO_Pin: Pino que gerou a interrupção.
 * @retval Nenhum.
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == USER_Btn_Pin) {
        Bsp_StartDebounceTimer();
    }
}

/**
 * @brief Callback de recepção completa UART.
 * @param huart: Ponteiro para o handle da UART.
 * @retval Nenhum.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3) {
        uint8_t next_head = (g_rx_head + 1U) % RX_BUFFER_SIZE;
        if (next_head != g_rx_tail) {
            g_rx_buffer[g_rx_head] = g_current_rx_byte;
            g_rx_head = next_head;
        }
        HAL_UART_Receive_IT(&huart3, &g_current_rx_byte, 1U);
    }
}
