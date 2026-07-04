/**
 * @file    SerialCmd.h
 * @brief   Módulo de recepção e interpretação de comandos UART.
 */
#ifndef SERIAL_CMD_H
#define SERIAL_CMD_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Defines -------------------------------------------------------------------*/

/* Constantes ----------------------------------------------------------------*/
#define SERIAL_CMD_MAX_LEN  16U

/* Estruturas de Dados Locais ------------------------------------------------*/

/* Protótipos das Funções Públicas -------------------------------------------*/

/**
 * @brief Inicializa as variáveis do módulo SerialCmd.
 * @param Nenhum.
 * @retval Nenhum.
 */
void SerialCmd_Init(void);

/**
 * @brief Interpreta um byte recebido pela UART3.
 * @param byte: O byte recebido.
 * @retval Nenhum.
 */
void SerialCmd_HandleByte(uint8_t byte);

/**
 * @brief Obtém qual LED está atualmente selecionado (1, 2 ou 3).
 * @param Nenhum.
 * @retval Índice do LED selecionado (1, 2 ou 3).
 */
uint8_t SerialCmd_GetSelectedLed(void);

#endif /* SERIAL_CMD_H */
