/**************************************************************************************************
 * Archivo:   uHALdac.h
 * Breve:     Interfaz HAL para DACs
 * Autor:     Guillermo Caporaletti
 * Fecha:     creado en agosto 2023
 *************************************************************************************************/

/****** Librerías (includes) *********************************************************************/

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_dac.h"
#include "uOSAL.h"
#include "uHALdac.h"

/****** Definiciones privadas (macros) ***********************************************************/

#define N_MUESTRAS			105		// Muestras en un período de señal

/****** Definiciones privadas de tipos (private typedef) *****************************************/


/****** Definición de datos públicos *************************************************************/


/****** Definición de datos privados *************************************************************/

static DAC_HandleTypeDef hdac     [UHAL_CANTIDAD_DACS];
//static DMA_HandleTypeDef hdma_dac [UHAL_CANTIDAD_DACS];
static TIM_HandleTypeDef htim     [UHAL_CANTIDAD_DACS];

const uint32_t CANAL_DAC[UHAL_CANTIDAD_DACS]   = { DAC_CHANNEL_1,       DAC_CHANNEL_2       };
const uint32_t DISPARO_DAC[UHAL_CANTIDAD_DACS] = { DAC_TRIGGER_T2_TRGO, DAC_TRIGGER_T4_TRGO };
const uint32_t TEMPO_DAC[UHAL_CANTIDAD_DACS]   = { (uint32_t) TIM2,     (uint32_t) TIM4     };

/****** Declaración de funciones privadas ********************************************************/

static void U_DMA_Init ( dac_id_t );
static void U_DAC_Init ( dac_id_t );
static void U_TIM_Init ( dac_id_t );

/****** Definición de funciones públicas *********************************************************/

/*-------------------------------------------------------------------------------------------------
 * @brief Inicializa DMA, DAC2 y Timer.
 * @param DAC_NUM
 * @retval None
 */
void uHALdacdmaConfigurar ( dac_id_t NUM_DAC, dac_config_s * CONF_DAC)
{
	U_DMA_Init( NUM_DAC );
	U_DAC_Init( NUM_DAC );
	U_TIM_Init( NUM_DAC );
    HAL_TIM_Base_Start(&htim[NUM_DAC]);
}

/*-------------------------------------------------------------------------------------------------
 * @brief Comienza a enviar Datos a salida por DAC
 * @param Puntero a Datos y cantidad de datos Num_Datos
 * @retval None
 */
void uHALdacdmaComenzar ( dac_id_t NUM_DAC, uint32_t * DATOS, uint32_t NUM_DATOS)
{
	HAL_DAC_Start_DMA( &hdac[NUM_DAC],
			           CANAL_DAC[NUM_DAC],
					   DATOS,
					   NUM_DATOS,
					   DAC_ALIGN_12B_R); // TODO el último parámetro es para revisar
}

/*-------------------------------------------------------------------------------------------------
  * @brief Para el envío de Datos a salida por DAC
  * @param None
  * @retval None
  */
void uHALdacdmaParar ( dac_id_t NUM_DAC)
{
	HAL_DAC_Stop_DMA( &hdac[NUM_DAC],
			          CANAL_DAC[NUM_DAC] );
}

/****** Definición de funciones privadas *********************************************************/

/*-------------------------------------------------------------------------------------------------
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void U_DAC_Init( dac_id_t NUM_DAC )
{
  DAC_ChannelConfTypeDef sConfig = {0};

  /** DAC Initialization   */
  hdac[NUM_DAC].Instance = DAC;
  if ( HAL_DAC_Init(&hdac[NUM_DAC]) != HAL_OK )
  {
	  uoHuboError();
  }

  /** DAC channel OUT config   */
  sConfig.DAC_Trigger = DISPARO_DAC[NUM_DAC];
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac[NUM_DAC], &sConfig, CANAL_DAC[NUM_DAC]) != HAL_OK)
  {
	  uoHuboError();
  }
}

/*-------------------------------------------------------------------------------------------------
  * @brief TIM Initialization Function
  * @param None
  * @retval None
  */
static void U_TIM_Init( dac_id_t NUM_DAC )
{

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim[NUM_DAC].Instance = (TIM_TypeDef *) TEMPO_DAC[NUM_DAC] ;
  htim[NUM_DAC].Init.Prescaler = 0;				// TODO revisar la frecuencia
  htim[NUM_DAC].Init.CounterMode = TIM_COUNTERMODE_UP;
  htim[NUM_DAC].Init.Period = 7;
  htim[NUM_DAC].Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim[NUM_DAC].Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if ( HAL_TIM_Base_Init(&htim[NUM_DAC]) != HAL_OK )
  {
    uoHuboError();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if ( HAL_TIM_ConfigClockSource(&htim[NUM_DAC], &sClockSourceConfig) != HAL_OK )
  {
    uoHuboError();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim[NUM_DAC], &sMasterConfig) != HAL_OK)
  {
    uoHuboError();
  }
}

/**
  * Enable DMA controller clock
  */
static void U_DMA_Init( dac_id_t NUM_DAC )
{
	switch (NUM_DAC) {
	case UHAL_DAC_1:
		__HAL_RCC_DMA1_CLK_ENABLE();
		HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
		break;
	case UHAL_DAC_2:
		__HAL_RCC_DMA2_CLK_ENABLE();
		HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
		break;
	default:
	    uoHuboError();
	    break;
	}
}

/****************************************************************** FIN DE ARCHIVO ***************/
