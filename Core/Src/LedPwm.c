/**
 * @file    LedPwm.c
 * @addtogroup LEDPWM
 * @{
 * @brief   Implementacao do modulo de controle de intensidade dos LEDs.
 *
 * Armazena o duty cycle de cada LED e converte o valor percentual para o
 * valor de registrador PWM exigido pela BSP antes de aplicar o sinal.
 */

/* Includes ------------------------------------------------------------------*/
#include "LedPwm.h"
#include "Bsp.h"

/* Defines Locais ------------------------------------------------------------*/

/* Constantes ----------------------------------------------------------------*/

/* Variaveis Locais ----------------------------------------------------------*/

// Vetor de duty cycles: indices 1, 2, 3 correspondem aos LEDs 1, 2 e 3
// O indice 0 nao e utilizado (LEDs indexados a partir de 1)
static uint32_t g_led_duty[4] = {0U, 0U, 0U, 0U};

/* Prototipos Locais ---------------------------------------------------------*/

/* Funcoes Publicas ----------------------------------------------------------*/

/**
 * @brief   Inicializa as variaveis internas do modulo LedPwm.
 * @details Zera o duty cycle armazenado de todos os tres LEDs.
 * @param   Nenhum.
 * @retval  Nenhum.
 */
void LedPwm_Init(void)
{
    // Zera o duty de cada LED individualmente (indice 0 reservado)
    g_led_duty[1] = 0U;
    g_led_duty[2] = 0U;
    g_led_duty[3] = 0U;
}

/**
 * @brief   Aplica a intensidade percentual ao LED atualmente selecionado.
 * @details Converte o valor de porcentagem (0-100%) para o valor de
 *          registrador PWM (0-BSP_PWM_MAX_VALUE) e aciona a BSP.
 * @param   percent      Intensidade desejada (unidade: %, faixa: 0 a 100).
 * @param   selected_led Indice do LED a ser controlado (faixa valida: 1, 2 ou 3).
 * @retval  Nenhum.
 */
void LedPwm_SetSelected(uint32_t percent, uint8_t selected_led)
{
    if (selected_led >= 1U && selected_led <= 3U) {
        // Armazena o duty percentual para consulta posterior
        g_led_duty[selected_led] = percent;

        // Converte de 0-100% para 0-999 (resolucao do registrador CCR)
        uint32_t duty_pwm = (percent * BSP_PWM_MAX_VALUE) / LED_PWM_MAX_PERCENT;
        Bsp_SetPwmDuty(selected_led, duty_pwm);
    }
}

/**
 * @brief   Retorna o duty cycle atual de um LED especifico.
 * @details Le o valor armazenado internamente, nao faz leitura de hardware.
 * @param   led_index Indice do LED consultado (faixa valida: 1, 2 ou 3).
 * @retval  Duty cycle atual do LED (unidade: %, faixa: 0 a 100).
 *          Retorna 0 se o indice for invalido.
 */
uint32_t LedPwm_GetDuty(uint8_t led_index)
{
    if (led_index >= 1U && led_index <= 3U) {
        return g_led_duty[led_index];
    }
    // Indice invalido: retorna zero como valor seguro
    return 0U;
}

/* Funcoes Locais ------------------------------------------------------------*/

/** @} */
