/**
 * @file    Bsp.h
 * @brief   Acesso ao hardware: ADC, timers, EXTI, USART3.
 */
#ifndef BSP_H
#define BSP_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdint.h>

/* Defines -------------------------------------------------------------------*/

/* Constantes ----------------------------------------------------------------*/
#define BSP_ADC_MAX_VALUE    4095U
#define BSP_PWM_MAX_VALUE    999U

/* Estruturas de Dados Locais ------------------------------------------------*/

/* Protótipos das Funções Públicas -------------------------------------------*/

/**
 * @brief Inicializa os periféricos da BSP.
 * @param Nenhum.
 * @retval Nenhum.
 */
void Bsp_Init(void);

/**
 * @brief Inicia o timer de amostragem (5 ms).
 * @param Nenhum.
 * @retval Nenhum.
 */
void Bsp_StartSamplingTimer(void);

/**
 * @brief Inicia o timer de debounce para o botão.
 * @param Nenhum.
 * @retval Nenhum.
 */
void Bsp_StartDebounceTimer(void);

/**
 * @brief Para o timer de debounce.
 * @param Nenhum.
 * @retval Nenhum.
 */
void Bsp_StopDebounceTimer(void);

/**
 * @brief Inicia a geração de PWM para os LEDs.
 * @param Nenhum.
 * @retval Nenhum.
 */
void Bsp_StartPwm(void);

/**
 * @brief Ajusta o duty cycle de um canal PWM.
 * @param channel: 1, 2 ou 3 representando o LED correspondente.
 * @param duty: Valor do duty cycle (0 a BSP_PWM_MAX_VALUE).
 * @retval Nenhum.
 */
void Bsp_SetPwmDuty(uint8_t channel, uint32_t duty);

/**
 * @brief Transmite uma string formatada pela USART3.
 * @param msg: Ponteiro para a string a ser enviada.
 * @param len: Tamanho da string.
 * @retval Nenhum.
 */
void Bsp_Print(const char *msg, uint16_t len);

/**
 * @brief Realiza a leitura do ADC1 via polling.
 * @param Nenhum.
 * @retval Valor lido do ADC.
 */
uint32_t Bsp_ReadAdc(void);

/**
 * @brief Verifica a flag de amostragem pendente.
 * @param Nenhum.
 * @retval 1 se houver amostragem pendente, 0 caso contrário.
 */
uint8_t Bsp_IsSamplingPending(void);

/**
 * @brief Limpa a flag de amostragem pendente.
 * @param Nenhum.
 * @retval Nenhum.
 */
void Bsp_ClearSamplingPending(void);

/**
 * @brief Verifica a flag de debounce concluído.
 * @param Nenhum.
 * @retval 1 se o debounce concluiu, 0 caso contrário.
 */
uint8_t Bsp_IsDebouncePending(void);

/**
 * @brief Limpa a flag de debounce concluído.
 * @param Nenhum.
 * @retval Nenhum.
 */
void Bsp_ClearDebouncePending(void);

/**
 * @brief Lê o byte recebido pela UART, se disponível.
 * @param out_byte: Ponteiro para armazenar o byte.
 * @retval 1 se um byte foi lido, 0 se não havia byte.
 */
uint8_t Bsp_GetRxByte(uint8_t *out_byte);

#endif /* BSP_H */
