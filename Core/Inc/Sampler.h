/**
 * @file    Sampler.h
 * @addtogroup SAMPLER
 * @{
 * @brief   Interface publica do modulo de aquisicao e filtragem do ADC.
 *
 * Acumula leituras brutas do ADC e calcula a media aritmetica a cada
 * SAMPLER_NUM_SAMPLES amostras, disponibilizando o resultado em porcentagem.
 */

#ifndef SAMPLER_H
#define SAMPLER_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Defines -------------------------------------------------------------------*/

/* Constantes ----------------------------------------------------------------*/

/// Quantidade de amostras acumuladas antes do calculo da media (adimensional, faixa: >= 1)
#define SAMPLER_NUM_SAMPLES  100U

/// Valor maximo do ADC de 12 bits usado na normalizacao (adimensional, faixa: 0 a 4095)
#define SAMPLER_ADC_MAX      4095U

/* Estruturas de Dados Locais ------------------------------------------------*/

/* Prototipos das Funcoes Publicas -------------------------------------------*/

/**
 * @brief   Inicializa as variaveis internas do modulo Sampler.
 * @details Zera o acumulador, o contador de amostras, o resultado filtrado
 *          e a flag de nova media disponivel. Deve ser chamada antes do loop principal.
 * @param   Nenhum.
 * @retval  Nenhum.
 */
void Sampler_Init(void);

/**
 * @brief   Processa uma nova amostra do ADC.
 * @details Acumula o valor recebido e, ao atingir SAMPLER_NUM_SAMPLES amostras,
 *          calcula a media, normaliza para 0-100% e seta a flag de nova media.
 *          O acumulador e o contador sao zerados apos cada ciclo completo.
 * @param   adc_value Valor bruto lido do ADC (faixa: 0 a SAMPLER_ADC_MAX).
 * @retval  Nenhum.
 */
void Sampler_ProcessSample(uint32_t adc_value);

/**
 * @brief   Retorna o ultimo resultado filtrado em porcentagem.
 * @details O valor e atualizado a cada ciclo completo de SAMPLER_NUM_SAMPLES amostras.
 * @param   Nenhum.
 * @retval  Valor percentual filtrado (unidade: %, faixa: 0 a 100).
 */
uint32_t Sampler_GetPercent(void);

/**
 * @brief   Consulta se uma nova media esta disponivel para consumo.
 * @details A flag e setada ao fim de cada ciclo de amostragem completo.
 *          Deve ser verificada pelo loop principal antes de usar o resultado.
 * @param   Nenhum.
 * @retval  1 se ha nova media disponivel, 0 caso contrario.
 */
uint8_t Sampler_IsNewAverageAvailable(void);

/**
 * @brief   Limpa a flag de nova media disponivel.
 * @details Deve ser chamada apos o consumo do resultado pelo loop principal.
 * @param   Nenhum.
 * @retval  Nenhum.
 */
void Sampler_ClearNewAverage(void);

#endif /* SAMPLER_H */

/** @} */
