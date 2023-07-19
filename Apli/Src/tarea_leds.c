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
		uLedActualizarTodo();
		//uLedActualizar( 0 );
		//uLedActualizar( 1 );
		vTaskDelay( LED_DELTAT );
	}
}

/*-------------------------------------------------------------------------------------------------
 * @brief	Cambia el modo de un led de forma segura
 * @param	Identificador del led y modo deseado
 */
bool TareaLedsModo (led_id_t LED, led_modo_t MODO)
{
	bool RET = false;
	RET = uLedConfigurarModo ( LED, MODO );
	return RET;
}

/****************************************************************** FIN DE ARCHIVO ***************/
