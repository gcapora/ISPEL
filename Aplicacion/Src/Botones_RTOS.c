/**************************************************************************************************
* @file		tarea_botones.c
* @author	Guillermo Caporaletti
* @brief	Tarea que controla todos los botones.
* @date		agosto de 2023
*
**************************************************************************************************/


/****** Librerías (includes) *********************************************************************/

#include <uBotones.h>
#include "Botones_RTOS.h"

/****** Definiciones privadas (macros) ***********************************************************/


/****** Definiciones privadas de tipos (private typedef) *****************************************/


/****** Definición de datos privados *************************************************************/

SemaphoreHandle_t MutexManejadorBTNS;	// Esta variable privada global administra el manejo del
													// semáforo que administra el acceso a los botones.

/****** Definición de datos públicos *************************************************************/


/****** Declaración de funciones privadas ********************************************************/


/****** Definición de funciones privadas *********************************************************/


/****** Definición de funciones públicas *********************************************************/

/*-------------------------------------------------------------------------------------------------
 * @brief	Inicializa el módulo de manejo de BOTONES
 * @param	Ninguno
 * @retval	true, si no hubo problemas
 */
bool BotonesRTOS_Inicializar (void)
{
	// Creamos un semáforo Mutex para el acceso concurrente a leds
	MutexManejadorBTNS = xSemaphoreCreateMutex();
	configASSERT ( NULL != MutexManejadorBTNS );
	// Terminada la inicialización...
	return true;
}

/*-------------------------------------------------------------------------------------------------
 * @brief	Tarea que actualiza el estado de los botones
 * @param
 */

bool BotonesRTOS_ActualizarTodo ( TickType_t Espera )
{
	bool RET = false;
	if( pdTRUE == xSemaphoreTake(MutexManejadorBTNS,Espera) ) {
		uBotonActualizarTodo();
		xSemaphoreGive( MutexManejadorBTNS );
		RET = true;
	}
	return RET;
}

/*-------------------------------------------------------------------------------------------------
 * @brief	Inicializa un boton de forma segura
 * @param	PIN = identificador de HW del boton
 * @retval	true, si no hubo problemas
 */
boton_id_t BotonesRTOS_InicializarBoton ( hal_pin_id_t PIN )
{
	boton_id_t RET = ERROR_LED;
	xSemaphoreTake( MutexManejadorBTNS, portMAX_DELAY );
	RET = uBotonInicializar( PIN );
	xSemaphoreGive( MutexManejadorBTNS );
	return RET;
}

/*-------------------------------------------------------------------------------------------------
 * @brief	Verifica si se produjo un FLANCO hacia botón PRESIONADO
 * @param	BOTON = identificador del boton
 * @retval	true si hubo FLANCO hacia boón PRESIONADO
 * 			Utiliza un semáforo para leer de forma segura
 */
bool BotonesRTOS_BotonFlancoPresionado ( boton_id_t BOTON )
{
	bool RET = false;
	xSemaphoreTake( MutexManejadorBTNS, portMAX_DELAY );
	RET = uBotonFlancoPresionando ( BOTON );
	xSemaphoreGive( MutexManejadorBTNS );
	return RET;
}

/*-------------------------------------------------------------------------------------------------
 * @brief	Verifica si se produjo un PRESIONADO LARGO de BOTON
 * @param	BOTON = identificador del boton
 * @retval	true si hubo PRESIONADO LARGO
 * 			Utiliza un semáforo para leer de forma segura
 */
bool BotonesRTOS_BotonPresionadoLargo ( boton_id_t BOTON )
{
	bool RET = false;
	xSemaphoreTake( MutexManejadorBTNS, portMAX_DELAY );
	RET = uBotonPresionadoLargo ( BOTON );
	xSemaphoreGive( MutexManejadorBTNS );
	return RET;
}

/****************************************************************** FIN DE ARCHIVO ***************/
