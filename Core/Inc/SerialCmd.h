/**
 * @file    SerialCmd.h
 * @addtogroup SERIALCMD
 * @{
 * @brief   Interface publica do modulo de recepcao e interpretacao de comandos UART.
 *
 * Recebe bytes individuais, monta comandos de texto e interpreta os tokens
 * validos (LED1, LED2, LED3) para selecionar o LED a ser controlado.
 */

#ifndef SERIAL_CMD_H
#define SERIAL_CMD_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Defines -------------------------------------------------------------------*/

/* Constantes ----------------------------------------------------------------*/

/// Tamanho maximo do buffer de comando incluindo o terminador nulo (unidade: bytes)
#define SERIAL_CMD_MAX_LEN  16U

/* Estruturas de Dados Locais ------------------------------------------------*/

/* Prototipos das Funcoes Publicas -------------------------------------------*/

/**
 * @brief   Inicializa as variaveis internas do modulo SerialCmd.
 * @details Zera o buffer de comando, o indice e define LED1 como selecao padrao.
 *          Deve ser chamada uma unica vez antes do loop principal.
 * @param   Nenhum.
 * @retval  Nenhum.
 */
void SerialCmd_Init(void);

/**
 * @brief   Processa um byte recebido pela UART3 e monta o comando.
 * @details Acumula bytes no buffer ate receber um terminador de linha (CR ou LF).
 *          Ao receber o terminador, interpreta o comando acumulado e atualiza
 *          o LED selecionado. Comandos maiores que SERIAL_CMD_MAX_LEN sao descartados.
 *          Comandos reconhecidos: "LED1", "LED2", "LED3".
 * @param   byte Byte recebido pela UART (valor ASCII).
 * @retval  Nenhum.
 */
void SerialCmd_HandleByte(uint8_t byte);

/**
 * @brief   Retorna o indice do LED atualmente selecionado.
 * @details O valor e atualizado sempre que um comando valido e recebido.
 *          O valor padrao apos a inicializacao e 1 (LED1).
 * @param   Nenhum.
 * @retval  Indice do LED selecionado (faixa: 1, 2 ou 3).
 */
uint8_t SerialCmd_GetSelectedLed(void);

#endif /* SERIAL_CMD_H */

/** @} */
