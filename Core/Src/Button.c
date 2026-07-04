/**
 * @file    Button.c
 * @addtogroup BUTTON
 * @{
 * @brief   Implementacao do modulo de controle do botao e debounce.
 *
 * Mantem o estado logico de congelamento do sistema e processa o evento
 * de pressao valida do botao apos o periodo de debounce concluido pela BSP.
 */

/* Includes ------------------------------------------------------------------*/
#include "Button.h"

/* Defines Locais ------------------------------------------------------------*/

/* Constantes ----------------------------------------------------------------*/

/* Variaveis Locais ----------------------------------------------------------*/

// Estado de congelamento: 1 = sistema congelado, 0 = operacao normal
static uint8_t g_frozen = 0U;

/* Prototipos Locais ---------------------------------------------------------*/

/* Funcoes Publicas ----------------------------------------------------------*/

/**
 * @brief   Inicializa as variaveis internas do modulo Button.
 * @details Define o estado inicial como nao-congelado.
 * @param   Nenhum.
 * @retval  Nenhum.
 */
void Button_Init(void)
{
    // Sistema inicia em operacao normal (nao-congelado)
    g_frozen = 0U;
}

/**
 * @brief   Processa o evento de pressao valida e alterna o estado de congelamento.
 * @details Implementa um toggle: cada pressao valida inverte o estado atual.
 * @param   Nenhum.
 * @retval  Nenhum.
 */
void Button_HandleDebounce(void)
{
    if (g_frozen == 0U) {
        // Primeira pressao: congela o sistema
        g_frozen = 1U;
    } else {
        // Segunda pressao: retoma a operacao normal
        g_frozen = 0U;
    }
}

/**
 * @brief   Consulta se o sistema esta no estado congelado.
 * @param   Nenhum.
 * @retval  1 se o sistema esta congelado, 0 se esta em operacao normal.
 */
uint8_t Button_IsFrozen(void)
{
    return g_frozen;
}

/* Funcoes Locais ------------------------------------------------------------*/

/** @} */
