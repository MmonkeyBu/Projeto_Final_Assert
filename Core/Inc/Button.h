/**
 * @file    Button.h
 * @addtogroup BUTTON
 * @{
 * @brief   Interface publica do modulo de controle do botao e debounce.
 *
 * Gerencia o estado logico do botao do usuario apos a validacao do debounce.
 * Expoe o estado de congelamento (frozen) que suspende a atualizacao dos LEDs.
 */

#ifndef BUTTON_H
#define BUTTON_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Defines -------------------------------------------------------------------*/

/* Constantes ----------------------------------------------------------------*/

/* Estruturas de Dados Locais ------------------------------------------------*/

/* Prototipos das Funcoes Publicas -------------------------------------------*/

/**
 * @brief   Inicializa as variaveis internas do modulo Button.
 * @details Define o estado inicial como nao-congelado (frozen = 0).
 *          Deve ser chamada uma unica vez antes do loop principal.
 * @param   Nenhum.
 * @retval  Nenhum.
 */
void Button_Init(void);

/**
 * @brief   Processa o evento de pressao valida e alterna o estado de congelamento.
 * @details Deve ser chamada no loop principal quando a flag de debounce
 *          concluido estiver ativa. Alterna entre frozen=1 e frozen=0 a
 *          cada pressao valida do botao.
 * @param   Nenhum.
 * @retval  Nenhum.
 */
void Button_HandleDebounce(void);

/**
 * @brief   Consulta se o sistema esta no estado congelado.
 * @details Quando congelado, o loop principal nao deve atualizar os LEDs
 *          nem processar novas amostras do ADC.
 * @param   Nenhum.
 * @retval  1 se o sistema esta congelado, 0 se esta em operacao normal.
 */
uint8_t Button_IsFrozen(void);

#endif /* BUTTON_H */

/** @} */
