/**************************************************************************************************
 * Archivo: uHAL.c
 * Breve:
 * Fecha:	Creado en abril 2024
 * Autor:	Guillermo F. Caporaletti
 *
 *************************************************************************************************/

/****** Librerías (includes) *********************************************************************/

#include "uHAL.h"

/****** Definiciones privadas (macros) ***********************************************************/


/****** Definiciones privadas de tipos (private typedef) *****************************************/


/****** Definición de datos privados *************************************************************/


/****** Importación de datos públicos ************************************************************/


/****** Declaración de funciones privadas ********************************************************/


/****** Definición de funciones públicas *********************************************************/

/**------------------------------------------------------------------------------------------------
  * @brief
  * @param
  * @retval
  */
bool uHALinicializar ( void )
{
  bool control = true;

  // Empezamos por inicializar uOSAL:
  if ( false == uoInicializar () ) control = false;

  // Acá estarán todos los módulos que necesiten inicialización general.
  //if ( uHALadcInicializar () != true ) control = false;

  return control;
}

void UHAL_GPIO_ADC_INICIALIZAR  (GPIO_TypeDef * GPIOx, uint32_t PIN)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if (GPIOx==GPIOA) {
		__HAL_RCC_GPIOA_CLK_ENABLE();

	} else if (GPIOx==GPIOB) {
		__HAL_RCC_GPIOB_CLK_ENABLE();

	} else if (GPIOx==GPIOC) {
		__HAL_RCC_GPIOC_CLK_ENABLE();

	} else if (GPIOx==GPIOF) {
		__HAL_RCC_GPIOF_CLK_ENABLE();
	}

	GPIO_InitStruct.Pin  = PIN;
   GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}



/****************************************************************** FIN DE ARCHIVO ***************/
