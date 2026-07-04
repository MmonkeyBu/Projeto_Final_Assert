/**
 * @file    Sampler.c
 * @addtogroup SAMPLER
 * @{
 * @brief   Implementacao do modulo de aquisicao e filtragem do ADC.
 *
 * Implementa o filtro de media movel por acumulacao: acumula SAMPLER_NUM_SAMPLES
 * leituras brutas do ADC e calcula a media ao fim de cada ciclo, normalizando
 * o resultado para uma escala de 0 a 100%.
 */

/* Includes ------------------------------------------------------------------*/
#include "Sampler.h"

/* Defines Locais ------------------------------------------------------------*/

/* Constantes ----------------------------------------------------------------*/

/* Variaveis Locais ----------------------------------------------------------*/

// Contador de amostras coletadas no ciclo atual
static uint32_t g_sample_count = 0U;

// Acumulador das leituras brutas do ADC no ciclo atual
static uint32_t g_accumulator = 0U;

// Resultado percentual da ultima media calculada (unidade: %)
static uint32_t g_filtered_percent = 0U;

// Flag que indica que uma nova media foi calculada e esta pronta para consumo
static uint8_t g_new_average = 0U;

/* Prototipos Locais ---------------------------------------------------------*/

/* Funcoes Publicas ----------------------------------------------------------*/

/**
 * @brief   Inicializa as variaveis internas do modulo Sampler.
 * @param   Nenhum.
 * @retval  Nenhum.
 */
void Sampler_Init(void)
{
    g_sample_count    = 0U;
    g_accumulator     = 0U;
    g_filtered_percent = 0U;
    g_new_average     = 0U;
}

/**
 * @brief   Processa uma nova amostra do ADC.
 * @details Acumula o valor e, ao completar SAMPLER_NUM_SAMPLES, calcula
 *          a media, normaliza para porcentagem e seta a flag de nova media.
 * @param   adc_value Valor bruto lido do ADC (faixa: 0 a SAMPLER_ADC_MAX).
 * @retval  Nenhum.
 */
void Sampler_ProcessSample(uint32_t adc_value)
{
    g_accumulator += adc_value;
    g_sample_count++;

    // Verifica se o numero de amostras para a media foi atingido
    if (g_sample_count >= SAMPLER_NUM_SAMPLES) {
        uint32_t average_value = g_accumulator / SAMPLER_NUM_SAMPLES;

        // Zera o acumulador e o contador para o proximo ciclo
        g_sample_count = 0U;
        g_accumulator  = 0U;

        // Normaliza o valor medio para a escala de 0 a 100%
        g_filtered_percent = (average_value * 100U) / SAMPLER_ADC_MAX;

        // Satura em 100% para evitar valores espurios por arredondamento
        if (g_filtered_percent > 100U) {
            g_filtered_percent = 100U;
        }

        // Sinaliza que o resultado esta pronto para consumo
        g_new_average = 1U;
    }
}

/**
 * @brief   Retorna o ultimo resultado filtrado em porcentagem.
 * @param   Nenhum.
 * @retval  Valor percentual filtrado (unidade: %, faixa: 0 a 100).
 */
uint32_t Sampler_GetPercent(void)
{
    return g_filtered_percent;
}

/**
 * @brief   Consulta se uma nova media esta disponivel para consumo.
 * @param   Nenhum.
 * @retval  1 se ha nova media disponivel, 0 caso contrario.
 */
uint8_t Sampler_IsNewAverageAvailable(void)
{
    return g_new_average;
}

/**
 * @brief   Limpa a flag de nova media disponivel.
 * @param   Nenhum.
 * @retval  Nenhum.
 */
void Sampler_ClearNewAverage(void)
{
    g_new_average = 0U;
}

/* Funcoes Locais ------------------------------------------------------------*/

/** @} */
