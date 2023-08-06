/**************************************************************************************************
* @file		tarea_botones.c
* @author	Guillermo Caporaletti
* @brief	Tarea que controla todos los botones.
* @date		agosto de 2023
*
**************************************************************************************************/


/****** Librerías (includes) *********************************************************************/

#include "tarea_botones.h"
#include "ubotones.h"

/****** Definiciones privadas (macros) ***********************************************************/

#define		LED_DELTAT			pdMS_TO_TICKS( 10UL )

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
bool TareaBotonesInicializar (void)
{
	// Creamos un semáforo Mutex para el acceso concurrente a leds
	MutexManejadorBTNS = xSemaphoreCreateMutex();
	configASSERT ( NULL != MutexManejadorBTNS );
	// Terminada la inicialización...
	return true;
}

/*-------------------------------------------------------------------------------------------------
 * @brief	Tarea que actualiza el estado de los botones
 * @param	Ninguno
 */
void TareaBotones ( void *pvParameters )
{
	/* Imprimir la tarea iniciada: */
	char *pcTaskName = (char *) pcTaskGetName( NULL );
	vPrintTwoStrings( pcTaskName, " esta ejecutandose." );

	/* Ciclo infinito, como la mayoría de las tareas: */
	for( ;; )
	{
		xSemaphoreTake( MutexManejadorBTNS, portMAX_DELAY );
		uBotonActualizarTodo();
		xSemaphoreGive( MutexManejadorBTNS );
		vTaskDelay( LED_DELTAT );
	}
}

/*-------------------------------------------------------------------------------------------------
 * @brief	Inicializa un boton de forma segura
 * @param	PIN = identificador de HW del boton
 * @retval	true, si no hubo problemas
 */
boton_id_t TareaBotones_InicializarBoton ( hal_pin_id_t PIN )
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
bool TareaBotones_BotonFlancoPresionado ( boton_id_t BOTON )
{
	bool RET = false;
	xSemaphoreTake( MutexManejadorBTNS, portMAX_DELAY );
	RET = uBotonFlancoPresionado ( BOTON );
	xSemaphoreGive( MutexManejadorBTNS );
	return RET;
}

/*-------------------------------------------------------------------------------------------------
 * @brief	Verifica si se produjo un PRESIONADO LARGO de BOTON
 * @param	BOTON = identificador del boton
 * @retval	true si hubo PRESIONADO LARGO
 * 			Utiliza un semáforo para leer de forma segura
 */
bool TareaBotones_BotonPresionadoLargo ( boton_id_t BOTON )
{
	bool RET = false;
	xSemaphoreTake( MutexManejadorBTNS, portMAX_DELAY );
	RET = uBotonPresionadoLargo ( BOTON );
	xSemaphoreGive( MutexManejadorBTNS );
	return RET;
}

/****************************************************************** FIN DE ARCHIVO ***************/
