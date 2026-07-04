/**
 * @file    LedPwm.h
 * @brief   Módulo de controle da intensidade dos LEDs.
 */
#ifndef LED_PWM_H
#define LED_PWM_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Defines -------------------------------------------------------------------*/

/* Constantes ----------------------------------------------------------------*/
#define LED_PWM_MIN_PERCENT  0U
#define LED_PWM_MAX_PERCENT  100U

/* Estruturas de Dados Locais ------------------------------------------------*/

/* Protótipos das Funções Públicas -------------------------------------------*/

/**
 * @brief Inicializa as variáveis do módulo LedPwm.
 * @param Nenhum.
 * @retval Nenhum.
 */
void LedPwm_Init(void);

/**
 * @brief Aplica o valor do potenciômetro ao LED selecionado.
 * @param percent: Intensidade de 0 a 100%.
 * @param selected_led: Qual LED está selecionado (1, 2 ou 3).
 * @retval Nenhum.
 */
void LedPwm_SetSelected(uint32_t percent, uint8_t selected_led);

/**
 * @brief Obtém o duty atual de um LED específico.
 * @param led_index: Índice do LED (1, 2 ou 3).
 * @retval Duty atual (0 a 100%).
 */
uint32_t LedPwm_GetDuty(uint8_t led_index);

#endif /* LED_PWM_H */
