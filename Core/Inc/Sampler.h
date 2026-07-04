/**
 * @file    Sampler.h
 * @brief   Módulo de aquisição e filtragem do ADC.
 */
#ifndef SAMPLER_H
#define SAMPLER_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Defines -------------------------------------------------------------------*/

/* Constantes ----------------------------------------------------------------*/
#define SAMPLER_NUM_SAMPLES  100U
#define SAMPLER_ADC_MAX      4095U

/* Estruturas de Dados Locais ------------------------------------------------*/

/* Protótipos das Funções Públicas -------------------------------------------*/

/**
 * @brief Inicializa as variáveis do módulo Sampler.
 * @param Nenhum.
 * @retval Nenhum.
 */
void Sampler_Init(void);

/**
 * @brief Processa uma nova amostra, acumulando para cálculo da média.
 * @param adc_value: Valor amostrado pelo ADC.
 * @retval Nenhum.
 */
void Sampler_ProcessSample(uint32_t adc_value);

/**
 * @brief Obtém o valor em porcentagem resultante da última filtragem.
 * @param Nenhum.
 * @retval Valor percentual (0 a 100).
 */
uint32_t Sampler_GetPercent(void);

/**
 * @brief Verifica se uma nova média foi calculada.
 * @param Nenhum.
 * @retval 1 se houver nova média, 0 caso contrário.
 */
uint8_t Sampler_IsNewAverageAvailable(void);

/**
 * @brief Limpa a flag de nova média disponível.
 * @param Nenhum.
 * @retval Nenhum.
 */
void Sampler_ClearNewAverage(void);

#endif /* SAMPLER_H */
