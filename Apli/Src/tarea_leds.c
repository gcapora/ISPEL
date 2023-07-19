/**************************************************************************************************
* @file		tarea_leds.c
* @author	Guillermo Caporaletti
* @brief	Tarea que controla todos los leds.
* @date		julio de 2023
*
**************************************************************************************************/


/****** Librerías (includes) *********************************************************************/

#include "tarea_leds.h"
#include "uleds.h"

/****** Definiciones privadas (macros) ***********************************************************/

#define		LED_DELTAT			pdMS_TO_TICKS( 1UL )

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
bool TareaLedsInicializar (void)
{
	// Creamos un semáforo Mutex para el acceso concurrente a leds
	MutexManejador = xSemaphoreCreateMutex();
	configASSERT ( NULL != MutexManejador );
	// Terminada la inicialización...
	return true;
}

/*-------------------------------------------------------------------------------------------------
 * @brief  Tarea que actualiza el estado de los leds
 * @param	Ninguno
 */
void TareaLeds( void *pvParameters )
{
	/* Imprimir la tarea iniciada: */
	char *pcTaskName = (char *) pcTaskGetName( NULL );
	vPrintTwoStrings( pcTaskName, " esta ejecutandose." );

	/* Ciclo infinito, como la mayoría de las tareas: */
	for( ;; )
	{
		xSemaphoreTake( MutexManejador, portMAX_DELAY );
		uLedActualizarTodo();
		xSemaphoreGive( MutexManejador );
		vTaskDelay( LED_DELTAT );
	}
}

/*-------------------------------------------------------------------------------------------------
 * @brief	Cambia el modo de un led de forma segura
 * @param	Identificador del led y modo deseado
 * @retval	true, si no hubo problemas
 */
led_id_t TareaLeds_InicializarLed ( hal_pin_id_t PIN )
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
bool TareaLeds_ModoLed (led_id_t LED, led_modo_t MODO)
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
bool TareaLeds_EncenderLed ( led_id_t LED )
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
bool TareaLeds_ApagarLed ( led_id_t LED )
{
	bool RET = false;
	xSemaphoreTake( MutexManejador, portMAX_DELAY );
	RET = uLedApagar ( LED );
	xSemaphoreGive( MutexManejador );
	return RET;
}

/****************************************************************** FIN DE ARCHIVO ***************/
