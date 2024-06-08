/**************************************************************************************************
* Archivo:	uHALconfig.h
* Breve:		Configuración de aplicación específica para los módulos uHAL
* Fecha:
* Autor:		Guillermo F. Caporaletti
*
**************************************************************************************************/

#ifndef ISPEL_UHAL_CONFIG_H_
#define ISPEL_UHAL_CONFIG_H_

/****** Librerías (includes) *********************************************************************/


/****** Definiciones públicas (macros) ***********************************************************/

/* Temporizadores -------------------------------------------------------------------------------*/
#define UHAL_MICROSEGUNDOS_TEMPO    UOSAL_MICROSEGUNDOS_TEMPO  // Para funcion uMicrosegundos
#define UHAL_ADC1_TEMPO             TIM3
#define UHAL_DAC1_TEMPO             TIM2
#define UHAL_DAC2_TEMPO             TIM4
#define UHAL_MAP_PE5_TEMPO          TIM9  // Utiliza TIM9_CH1 ----> PE5 (ver uHALmap.c)

/* ADC ------------------------------------------------------------------------------------------*/
#define UHAL_CANTIDAD_ADCS          2
#define UHAL_ADC1_ORIGEN            ADC_EXTERNALTRIGCONV_T3_TRGO   // Debe coincidir con UHAL_ADC1_TEMPO
#define UHAL_CANTIDAD_CANALES_ADCS  2
#define UHAL_ADC1_CANAL_1           ADC_CHANNEL_9
#define UHAL_ADC1_CANAL_2				ADC_CHANNEL_12
#define UHAL_ADC2_CANAL_1           ADC_CHANNEL_3
#define UHAL_ADC2_CANAL_2				ADC_CHANNEL_10

/* MAP (Modulación de Ancho de Pulso) -----------------------------------------------------------*/
#define UHAL_CANTIDAD_MAP           1

/* Nombres específicos de pines -----------------------------------------------------------------*/
#define U_LED_VERDE_EP HAL_PIN_PB0
#define U_LED_AZUL_EP  HAL_PIN_PB7
#define U_LED_ROJO_EP  HAL_PIN_PB14
#define U_BOTON_EP     HAL_PIN_PC13

/****** Definiciones públicas de tipos de datos (public typedef) *********************************/


/****** Declaraciones de datos externos **********************************************************/


/****** Declaración de funciones públicas ********************************************************/

void	UHAL_GPIO_ADC_INICIALIZAR  (GPIO_TypeDef * GPIOx, uint32_t PIN);

/*************************************************************************************************/
#endif /* ISPEL_UHAL_CONFIG_H_ */
/****************************************************************** FIN DE ARCHIVO ***************/
