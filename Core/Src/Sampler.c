/**
 * @file    Sampler.c
 * @brief   Implementação do módulo de aquisição e filtragem do ADC.
 */

/* Includes ------------------------------------------------------------------*/
#include "Sampler.h"

/* Defines Locais ------------------------------------------------------------*/

/* Constantes ----------------------------------------------------------------*/

/* Variáveis Locais ----------------------------------------------------------*/
static uint32_t g_sample_count = 0U;
static uint32_t g_accumulator = 0U;
static uint32_t g_filtered_percent = 0U;
static uint8_t g_new_average = 0U;

/* Protótipos Locais ---------------------------------------------------------*/

/* Funções Públicas ----------------------------------------------------------*/

/**
 * @brief Inicializa as variáveis do módulo Sampler.
 * @param Nenhum.
 * @retval Nenhum.
 */
void Sampler_Init(void)
{
    g_sample_count = 0U;
    g_accumulator = 0U;
    g_filtered_percent = 0U;
    g_new_average = 0U;
}

/**
 * @brief Processa uma nova amostra, acumulando para cálculo da média.
 * @param adc_value: Valor amostrado pelo ADC.
 * @retval Nenhum.
 */
void Sampler_ProcessSample(uint32_t adc_value)
{
    g_accumulator += adc_value;
    g_sample_count++;

    if (g_sample_count >= SAMPLER_NUM_SAMPLES) {
        uint32_t average_value = g_accumulator / SAMPLER_NUM_SAMPLES;
        g_sample_count = 0U;
        g_accumulator = 0U;

        g_filtered_percent = (average_value * 100U) / SAMPLER_ADC_MAX;
        if (g_filtered_percent > 100U) {
            g_filtered_percent = 100U;
        }

        g_new_average = 1U;
    }
}

/**
 * @brief Obtém o valor em porcentagem resultante da última filtragem.
 * @param Nenhum.
 * @retval Valor percentual (0 a 100).
 */
uint32_t Sampler_GetPercent(void)
{
    return g_filtered_percent;
}

/**
 * @brief Verifica se uma nova média foi calculada.
 * @param Nenhum.
 * @retval 1 se houver nova média, 0 caso contrário.
 */
uint8_t Sampler_IsNewAverageAvailable(void)
{
    return g_new_average;
}

/**
 * @brief Limpa a flag de nova média disponível.
 * @param Nenhum.
 * @retval Nenhum.
 */
void Sampler_ClearNewAverage(void)
{
    g_new_average = 0U;
}

/* Funções Locais ------------------------------------------------------------*/
