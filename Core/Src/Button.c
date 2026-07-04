/**
 * @file    Button.c
 * @brief   Implementação do módulo de controle do botão e debounce.
 */

/* Includes ------------------------------------------------------------------*/
#include "Button.h"

/* Defines Locais ------------------------------------------------------------*/

/* Constantes ----------------------------------------------------------------*/

/* Variáveis Locais ----------------------------------------------------------*/
static uint8_t g_frozen = 0U;

/* Protótipos Locais ---------------------------------------------------------*/

/* Funções Públicas ----------------------------------------------------------*/

/**
 * @brief Inicializa as variáveis do módulo Button.
 * @param Nenhum.
 * @retval Nenhum.
 */
void Button_Init(void)
{
    g_frozen = 0U;
}

/**
 * @brief Processa o fim do debounce e alterna o estado de congelamento.
 * @param Nenhum.
 * @retval Nenhum.
 */
void Button_HandleDebounce(void)
{
    if (g_frozen == 0U) {
        g_frozen = 1U;
    } else {
        g_frozen = 0U;
    }
}

/**
 * @brief Verifica se a atualização dos LEDs e leitura estão congeladas.
 * @param Nenhum.
 * @retval 1 se congelado, 0 se em operação normal.
 */
uint8_t Button_IsFrozen(void)
{
    return g_frozen;
}

/* Funções Locais ------------------------------------------------------------*/
