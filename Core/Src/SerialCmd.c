/**
 * @file    SerialCmd.c
 * @brief   Implementação do módulo de recepção e interpretação de comandos UART.
 */

/* Includes ------------------------------------------------------------------*/
#include "SerialCmd.h"
#include <string.h>

/* Defines Locais ------------------------------------------------------------*/

/* Constantes ----------------------------------------------------------------*/

/* Variáveis Locais ----------------------------------------------------------*/
static uint8_t g_command_buffer[SERIAL_CMD_MAX_LEN];
static uint8_t g_command_index = 0U;
static uint8_t g_selected_led = 1U;

/* Protótipos Locais ---------------------------------------------------------*/

/* Funções Públicas ----------------------------------------------------------*/

/**
 * @brief Inicializa as variáveis do módulo SerialCmd.
 * @param Nenhum.
 * @retval Nenhum.
 */
void SerialCmd_Init(void)
{
    memset(g_command_buffer, 0, sizeof(g_command_buffer));
    g_command_index = 0U;
    g_selected_led = 1U;
}

/**
 * @brief Interpreta um byte recebido pela UART3.
 * @param byte: O byte recebido.
 * @retval Nenhum.
 */
void SerialCmd_HandleByte(uint8_t byte)
{
    if ((byte == '\r') || (byte == '\n')) {
        if (g_command_index != 0U) {
            g_command_buffer[g_command_index] = '\0';
            
            if (strcmp((const char *)g_command_buffer, "LED1") == 0) {
                g_selected_led = 1U;
            } else if (strcmp((const char *)g_command_buffer, "LED2") == 0) {
                g_selected_led = 2U;
            } else if (strcmp((const char *)g_command_buffer, "LED3") == 0) {
                g_selected_led = 3U;
            }
        }
        g_command_index = 0U;
        memset(g_command_buffer, 0, sizeof(g_command_buffer));
    } else if (g_command_index < (SERIAL_CMD_MAX_LEN - 1U)) {
        g_command_buffer[g_command_index++] = byte;
    }
}

/**
 * @brief Obtém qual LED está atualmente selecionado (1, 2 ou 3).
 * @param Nenhum.
 * @retval Índice do LED selecionado (1, 2 ou 3).
 */
uint8_t SerialCmd_GetSelectedLed(void)
{
    return g_selected_led;
}

/* Funções Locais ------------------------------------------------------------*/
