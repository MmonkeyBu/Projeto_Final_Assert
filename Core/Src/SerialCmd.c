/**
 * @file    SerialCmd.c
 * @addtogroup SERIALCMD
 * @{
 * @brief   Implementacao do modulo de recepcao e interpretacao de comandos UART.
 *
 * Implementa uma maquina de estados simples que acumula bytes recebidos em um
 * buffer circular ate a chegada de um terminador de linha, momento em que o
 * comando e comparado com os tokens validos para selecao de LED.
 */

/* Includes ------------------------------------------------------------------*/
#include "SerialCmd.h"
#include <string.h>

/* Defines Locais ------------------------------------------------------------*/

/* Constantes ----------------------------------------------------------------*/

/* Variaveis Locais ----------------------------------------------------------*/

// Buffer para acumular os bytes do comando em construcao
static uint8_t g_command_buffer[SERIAL_CMD_MAX_LEN];

// Indice de escrita no buffer de comando
static uint8_t g_command_index = 0U;

// Indice do LED atualmente selecionado pelo ultimo comando valido (faixa: 1 a 3)
static uint8_t g_selected_led = 1U;

/* Prototipos Locais ---------------------------------------------------------*/

/* Funcoes Publicas ----------------------------------------------------------*/

/**
 * @brief   Inicializa as variaveis internas do modulo SerialCmd.
 * @details Zera o buffer e o indice. Define LED1 como selecao padrao.
 * @param   Nenhum.
 * @retval  Nenhum.
 */
void SerialCmd_Init(void)
{
    memset(g_command_buffer, 0, sizeof(g_command_buffer));
    g_command_index = 0U;
    // LED1 e a selecao padrao ao inicializar o sistema
    g_selected_led = 1U;
}

/**
 * @brief   Processa um byte recebido pela UART3 e monta o comando.
 * @details Acumula bytes ate receber CR ou LF, entao interpreta o comando.
 *          Comandos reconhecidos: "LED1", "LED2", "LED3".
 * @param   byte Byte recebido pela UART (valor ASCII).
 * @retval  Nenhum.
 */
void SerialCmd_HandleByte(uint8_t byte)
{
    if ((byte == '\r') || (byte == '\n')) {
        // Terminador recebido: processa somente se o buffer nao estiver vazio
        if (g_command_index != 0U) {
            // Adiciona terminador nulo para uso com funcoes de string
            g_command_buffer[g_command_index] = '\0';

            // Compara o comando recebido com os tokens validos
            if (strcmp((const char *)g_command_buffer, "LED1") == 0) {
                g_selected_led = 1U;
            } else if (strcmp((const char *)g_command_buffer, "LED2") == 0) {
                g_selected_led = 2U;
            } else if (strcmp((const char *)g_command_buffer, "LED3") == 0) {
                g_selected_led = 3U;
            }
            // Comandos nao reconhecidos sao silenciosamente descartados
        }
        // Limpa o buffer para o proximo comando, independentemente do resultado
        g_command_index = 0U;
        memset(g_command_buffer, 0, sizeof(g_command_buffer));
    } else if (g_command_index < (SERIAL_CMD_MAX_LEN - 1U)) {
        // Acumula o byte no buffer deixando espaco para o terminador nulo
        g_command_buffer[g_command_index++] = byte;
    }
    // Bytes recebidos apos o limite maximo sao descartados silenciosamente
}

/**
 * @brief   Retorna o indice do LED atualmente selecionado.
 * @param   Nenhum.
 * @retval  Indice do LED selecionado (faixa: 1, 2 ou 3).
 */
uint8_t SerialCmd_GetSelectedLed(void)
{
    return g_selected_led;
}

/* Funcoes Locais ------------------------------------------------------------*/

/** @} */
