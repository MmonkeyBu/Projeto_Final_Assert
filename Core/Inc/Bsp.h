/**
 * @file    Bsp.h
 * @addtogroup BSP
 * @{
 * @brief   Interface publica da camada de abstracao de hardware (BSP).
 *
 * Declara as funcoes de acesso ao ADC, timers de amostragem e debounce,
 * geracao de PWM e comunicacao via USART3.
 */

#ifndef BSP_H
#define BSP_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdint.h>

/* Defines -------------------------------------------------------------------*/

/* Constantes ----------------------------------------------------------------*/

/// Valor maximo de saida do ADC de 12 bits (adimensional, faixa: 0 a 4095)
#define BSP_ADC_MAX_VALUE    4095U

/// Valor maximo do registrador de comparacao do PWM (adimensional, faixa: 0 a 999)
#define BSP_PWM_MAX_VALUE    999U

/* Estruturas de Dados Locais ------------------------------------------------*/

/* Prototipos das Funcoes Publicas -------------------------------------------*/

/**
 * @brief   Inicializa os perifericos da BSP.
 * @details Inicia os canais de PWM com duty zero e habilita a recepcao
 *          de dados pela UART via interrupcao.
 * @param   Nenhum.
 * @retval  Nenhum.
 */
void Bsp_Init(void);

/**
 * @brief   Inicia o timer de amostragem periodica do ADC.
 * @details Utiliza o TIM2 configurado para disparar a cada 5 ms.
 * @param   Nenhum.
 * @retval  Nenhum.
 */
void Bsp_StartSamplingTimer(void);

/**
 * @brief   Inicia o timer de debounce do botao.
 * @details Utiliza o TIM6 configurado para um periodo de debounce de 20 ms.
 *          Deve ser chamado na borda de subida do pino do botao.
 * @param   Nenhum.
 * @retval  Nenhum.
 */
void Bsp_StartDebounceTimer(void);

/**
 * @brief   Para o timer de debounce.
 * @details Desabilita o TIM6 e sua interrupcao apos o fim do debounce.
 * @param   Nenhum.
 * @retval  Nenhum.
 */
void Bsp_StopDebounceTimer(void);

/**
 * @brief   Inicia a geracao de sinal PWM nos tres canais dos LEDs.
 * @details Habilita TIM3-CH3, TIM4-CH2 e TIM12-CH1.
 * @param   Nenhum.
 * @retval  Nenhum.
 */
void Bsp_StartPwm(void);

/**
 * @brief   Ajusta o duty cycle de um canal de PWM.
 * @details O valor de duty e saturado em BSP_PWM_MAX_VALUE caso ultrapasse
 *          o limite. O mapeamento de canais e: 1->TIM3, 2->TIM4, 3->TIM12.
 * @param   channel Canal do LED a ser ajustado (faixa valida: 1, 2 ou 3).
 * @param   duty    Valor do registrador de comparacao (faixa: 0 a BSP_PWM_MAX_VALUE).
 * @retval  Nenhum.
 */
void Bsp_SetPwmDuty(uint8_t channel, uint32_t duty);

/**
 * @brief   Transmite uma mensagem de texto pela USART3.
 * @details Operacao bloqueante com timeout infinito. Utilizar apenas
 *          fora de contexto de interrupcao.
 * @param   msg Ponteiro para o buffer de caracteres a ser transmitido.
 * @param   len Numero de bytes a transmitir (unidade: bytes).
 * @retval  Nenhum.
 */
void Bsp_Print(const char *msg, uint16_t len);

/**
 * @brief   Realiza a leitura do ADC1 por polling disparado por software.
 * @details Inicia a conversao, aguarda o resultado por ate 10 ms e
 *          retorna o valor bruto de 12 bits.
 * @param   Nenhum.
 * @retval  Valor bruto lido do ADC (faixa: 0 a 4095, adimensional).
 */
uint32_t Bsp_ReadAdc(void);

/**
 * @brief   Consulta se ha uma amostragem do ADC pendente de processamento.
 * @details A flag e setada pelo callback do TIM2 a cada 5 ms.
 * @param   Nenhum.
 * @retval  1 se ha amostragem pendente, 0 caso contrario.
 */
uint8_t Bsp_IsSamplingPending(void);

/**
 * @brief   Limpa a flag de amostragem pendente.
 * @details Deve ser chamada apos o processamento da amostra pelo modulo Sampler.
 * @param   Nenhum.
 * @retval  Nenhum.
 */
void Bsp_ClearSamplingPending(void);

/**
 * @brief   Consulta se o debounce do botao foi concluido.
 * @details A flag e setada pelo callback do TIM6 ao fim do periodo de debounce.
 * @param   Nenhum.
 * @retval  1 se o debounce concluiu, 0 caso contrario.
 */
uint8_t Bsp_IsDebouncePending(void);

/**
 * @brief   Limpa a flag de debounce concluido.
 * @details Deve ser chamada apos o tratamento do evento de botao pelo modulo Button.
 * @param   Nenhum.
 * @retval  Nenhum.
 */
void Bsp_ClearDebouncePending(void);

/**
 * @brief   Le um byte do buffer circular de recepcao da UART.
 * @details O buffer e preenchido pelo callback de interrupcao da USART3.
 *          Esta funcao e nao-bloqueante.
 * @param   out_byte Ponteiro para a variavel onde o byte lido sera armazenado.
 * @retval  1 se um byte foi lido com sucesso, 0 se o buffer estava vazio.
 */
uint8_t Bsp_GetRxByte(uint8_t *out_byte);

#endif /* BSP_H */

/** @} */
