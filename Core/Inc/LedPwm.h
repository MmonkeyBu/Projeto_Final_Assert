/**
 * @file    LedPwm.h
 * @addtogroup LEDPWM
 * @{
 * @brief   Interface publica do modulo de controle de intensidade dos LEDs.
 *
 * Converte o valor percentual do Sampler em valor de duty cycle e aplica
 * ao LED selecionado via BSP. Armazena internamente o duty de cada LED.
 */

#ifndef LED_PWM_H
#define LED_PWM_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Defines -------------------------------------------------------------------*/

/* Constantes ----------------------------------------------------------------*/

/// Intensidade minima aplicavel a um LED (unidade: %, faixa: 0 a 100)
#define LED_PWM_MIN_PERCENT  0U

/// Intensidade maxima aplicavel a um LED (unidade: %, faixa: 0 a 100)
#define LED_PWM_MAX_PERCENT  100U

/* Estruturas de Dados Locais ------------------------------------------------*/

/* Prototipos das Funcoes Publicas -------------------------------------------*/

/**
 * @brief   Inicializa as variaveis internas do modulo LedPwm.
 * @details Zera o duty cycle armazenado de todos os tres LEDs.
 *          Deve ser chamada uma unica vez antes do loop principal.
 * @param   Nenhum.
 * @retval  Nenhum.
 */
void LedPwm_Init(void);

/**
 * @brief   Aplica a intensidade percentual ao LED atualmente selecionado.
 * @details Converte o valor de porcentagem (0-100%) para o valor de
 *          registrador PWM (0-BSP_PWM_MAX_VALUE) e aciona a BSP.
 *          Somente o LED selecionado e atualizado; os demais permanecem inalterados.
 * @param   percent      Intensidade desejada (unidade: %, faixa: 0 a 100).
 * @param   selected_led Indice do LED a ser controlado (faixa valida: 1, 2 ou 3).
 * @retval  Nenhum.
 */
void LedPwm_SetSelected(uint32_t percent, uint8_t selected_led);

/**
 * @brief   Retorna o duty cycle atual de um LED especifico.
 * @details Le o valor armazenado internamente, nao faz leitura de hardware.
 * @param   led_index Indice do LED consultado (faixa valida: 1, 2 ou 3).
 * @retval  Duty cycle atual do LED (unidade: %, faixa: 0 a 100).
 *          Retorna 0 se o indice for invalido.
 */
uint32_t LedPwm_GetDuty(uint8_t led_index);

#endif /* LED_PWM_H */

/** @} */
