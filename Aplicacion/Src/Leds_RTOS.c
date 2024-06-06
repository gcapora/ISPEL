/**************************************************************************************************
* @file		tarea_leds.c
* @author	Guillermo Caporaletti
* @brief	Tarea que controla todos los leds.
* @date		julio de 2023
*
**************************************************************************************************/


/****** Librerías (includes) *********************************************************************/

#include <uLeds.h>
#include "Leds_RTOS.h"

/****** Definiciones privadas (macros) ***********************************************************/


/****** Definiciones privadas de tipos (private typedef) *****************************************/


/****** Definición de datos privados *************************************************************/

SemaphoreHandle_t MutexManejador;	// Esta variable global administra el manejo del semáforo
									// que administra el acceso a los leds.

/****** Definición de datos públicos *************************************************************/


/****** Declaración de funciones privadas ********************************************************/


/****** Definición de funciones privadas *********************************************************/


/****** Definición de funciones públicas *********************************************************/

/*-------------------------------------------------------------------------------------------------
 * @brief  Inicializa todos los leds a utilizar
 * @param	Ninguno
 * @retval true, si no hubo problemas
 */
bool LedsRTOS_Inicializar (void)
{
	// Creamos un semáforo Mutex para el acceso concurrente a leds
	MutexManejador = xSemaphoreCreateMutex();
	configASSERT ( NULL != MutexManejador );
	// Terminada la inicialización...
	return true;
}

/*-------------------------------------------------------------------------------------------------
 * @brief	Actualiza todos los leds de forma segura
 * @param	Tiempo que puede esperar a cumplir la tarea
 * @retval	true si tuvo éxito
 */
bool LedsRTOS_ActualizarTodos ( TickType_t xTicksToWait )
{
	bool RET = false;
	if (pdTRUE == xSemaphoreTake( MutexManejador, xTicksToWait )) {
		uLedActualizarTodo();
		RET = true;
		xSemaphoreGive( MutexManejador );
	}
	return RET;
}

/*-------------------------------------------------------------------------------------------------
 * @brief	Cambia el modo de un led de forma segura
 * @param	Identificador del led y modo deseado
 * @retval	true, si no hubo problemas
 */
led_id_t LedsRTOS_InicializarLed ( hal_pin_id_t PIN )
{
	led_id_t RET = ERROR_LED;
	xSemaphoreTake( MutexManejador, portMAX_DELAY );
	RET = uLedInicializar( PIN );
	xSemaphoreGive( MutexManejador );
	return RET;
}

/*-------------------------------------------------------------------------------------------------
 * @brief	Cambia el modo de un led de forma segura
 * @param	Identificador del led y modo deseado
 * @retval	true, si no hubo problemas
 */
bool LedsRTOS_ModoLed (led_id_t LED, led_modo_t MODO)
{
	bool RET = false;
	xSemaphoreTake( MutexManejador, portMAX_DELAY );
	RET = uLedConfigurarModo ( LED, MODO );
	xSemaphoreGive( MutexManejador );
	return RET;
}

/*-------------------------------------------------------------------------------------------------
 * @brief	Cambia el modo de un led de forma segura
 * @param	Identificador del led y modo deseado
 * @retval	true, si no hubo problemas
 */
bool LedsRTOS_EncenderLed ( led_id_t LED )
{
	bool RET = false;
	xSemaphoreTake( MutexManejador, portMAX_DELAY );
	RET = uLedEncender ( LED );
	xSemaphoreGive( MutexManejador );
	return RET;
}

/*-------------------------------------------------------------------------------------------------
 * @brief	Cambia el modo de un led de forma segura
 * @param	Identificador del led y modo deseado
 * @retval	true, si no hubo problemas
 */
bool LedsRTOS_InvertirLed ( led_id_t LED )
{
	bool RET = false;
	xSemaphoreTake( MutexManejador, portMAX_DELAY );

	if ( LED_APAGADO == uLedObtenerEstado(LED) ) {
		RET = uLedEncender(LED);
	} else {
		RET = uLedApagar(LED);
	}
	xSemaphoreGive( MutexManejador );
	return RET;
}

/*-------------------------------------------------------------------------------------------------
 * @brief	Cambia el modo de un led de forma segura
 * @param	Identificador del led y modo deseado
 * @retval	true, si no hubo problemas
 */
bool LedsRTOS_ApagarLed ( led_id_t LED )
{
	bool RET = false;
	xSemaphoreTake( MutexManejador, portMAX_DELAY );
	RET = uLedApagar ( LED );
	xSemaphoreGive( MutexManejador );
	return RET;
}

/*-------------------------------------------------------------------------------------------------
 * @brief	Indica si el LED indicado está encendido
 * @param	Identificador del led y modo deseado
 * @retval	true si está encendido
 */
bool LedsRTOS_LedEncendido ( led_id_t LED )
{
	bool RET = false;
	xSemaphoreTake( MutexManejador, portMAX_DELAY );
	RET = ( LED_ENCENDIDO == uLedObtenerEstado ( LED ) );
	xSemaphoreGive( MutexManejador );
	return RET;
}

/****************************************************************** FIN DE ARCHIVO ***************/
