/**
 * @file    LedPwm.c
 * @brief   Implementação do módulo de controle da intensidade dos LEDs.
 */

/* Includes ------------------------------------------------------------------*/
#include "LedPwm.h"
#include "Bsp.h"

/* Defines Locais ------------------------------------------------------------*/

/* Constantes ----------------------------------------------------------------*/

/* Variáveis Locais ----------------------------------------------------------*/
static uint32_t g_led_duty[4] = {0U, 0U, 0U, 0U};

/* Protótipos Locais ---------------------------------------------------------*/

/* Funções Públicas ----------------------------------------------------------*/

/**
 * @brief Inicializa as variáveis do módulo LedPwm.
 * @param Nenhum.
 * @retval Nenhum.
 */
void LedPwm_Init(void)
{
    g_led_duty[1] = 0U;
    g_led_duty[2] = 0U;
    g_led_duty[3] = 0U;
}

/**
 * @brief Aplica o valor do potenciômetro ao LED selecionado.
 * @param percent: Intensidade de 0 a 100%.
 * @param selected_led: Qual LED está selecionado (1, 2 ou 3).
 * @retval Nenhum.
 */
void LedPwm_SetSelected(uint32_t percent, uint8_t selected_led)
{
    if (selected_led >= 1U && selected_led <= 3U) {
        g_led_duty[selected_led] = percent;

        /* Converte de 0-100% para 0-999 */
        uint32_t duty_pwm = (percent * BSP_PWM_MAX_VALUE) / LED_PWM_MAX_PERCENT;
        Bsp_SetPwmDuty(selected_led, duty_pwm);
    }
}

/**
 * @brief Obtém o duty atual de um LED específico.
 * @param led_index: Índice do LED (1, 2 ou 3).
 * @retval Duty atual (0 a 100%).
 */
uint32_t LedPwm_GetDuty(uint8_t led_index)
{
    if (led_index >= 1U && led_index <= 3U) {
        return g_led_duty[led_index];
    }
    return 0U;
}

/* Funções Locais ------------------------------------------------------------*/
