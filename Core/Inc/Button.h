/**
 * @file    Button.h
 * @brief   Módulo de controle do botão e debounce.
 */
#ifndef BUTTON_H
#define BUTTON_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Defines -------------------------------------------------------------------*/

/* Constantes ----------------------------------------------------------------*/

/* Estruturas de Dados Locais ------------------------------------------------*/

/* Protótipos das Funções Públicas -------------------------------------------*/

/**
 * @brief Inicializa as variáveis do módulo Button.
 * @param Nenhum.
 * @retval Nenhum.
 */
void Button_Init(void);

/**
 * @brief Processa o fim do debounce e alterna o estado de congelamento.
 * @param Nenhum.
 * @retval Nenhum.
 */
void Button_HandleDebounce(void);

/**
 * @brief Verifica se a atualização dos LEDs e leitura estão congeladas.
 * @param Nenhum.
 * @retval 1 se congelado, 0 se em operação normal.
 */
uint8_t Button_IsFrozen(void);

#endif /* BUTTON_H */
