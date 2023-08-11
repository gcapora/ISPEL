/**************************************************************************************************
 * Archivo: uOSAL.c
 * Breve:	Capa de abstracción de sistema operativo (OSAL). Proyecto ISPEL.
 * Fecha:	Creado en agosto 2023
 * Autor:	Guillermo F. Caporaletti
 *
 *************************************************************************************************/

/****** Librerías (includes) *********************************************************************/

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"

/****** Definiciones privadas (macros) ***********************************************************/


/****** Definiciones privadas de tipos (private typedef) *****************************************/


/****** Definición de datos públicos *************************************************************/


/****** Declaración de funciones privadas ********************************************************/


/****** Definición de funciones privadas *********************************************************/


/****** Definición de funciones públicas *********************************************************/

/**------------------------------------------------------------------------------------------------
  * @brief  Me devuelve el tiempo en milisegundos
  * @param	Ninguno
  * @retval Milisegundo actual
  */
uint32_t uOSALmiliseg( void )
{
	return (uint32_t) xTaskGetTickCount () * 1000 / configTICK_RATE_HZ;
}

/*-------------------------------------------------------------------------------------------------
 * @brief	Maneja el error
 * @param	Ninguno
 * @retval  Ninguno
 */
void uManejaError (void)   // TODO que coincida con error de freertos
{
  __disable_irq();
  while (1)
  {
  }
}




/****************************************************************** FIN DE ARCHIVO ***************/
