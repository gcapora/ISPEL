/**************************************************************************************************
* @file		tarea_test_1.c
* @author	Guillermo Caporaletti
* @brief	Tarea para testear aplicación.
* @date		julio de 2023
*
**************************************************************************************************/


/****** Librerías (includes) *********************************************************************/

#include "tarea_test_1.h"

/****** Definiciones privadas (macros) ***********************************************************/

#define DELTAT_TEST			pdMS_TO_TICKS( 5000UL )

/****** Definiciones privadas de tipos (private typedef) *****************************************/


/****** Definición de datos privados *************************************************************/

led_id_t LedRojoEnPlaca, LedAzulEnPlaca;

/****** Definición de datos públicos *************************************************************/


/****** Declaración de funciones privadas ********************************************************/


/****** Definición de funciones privadas *********************************************************/


/****** Definición de funciones públicas *********************************************************/

/*-------------------------------------------------------------------------------------------------
 * @brief  Inicializa todos los leds a utilizar
 * @param	Ninguno
 * @retval true, si no hubo problemas
 */
bool TareaTestInicializar (void) {

	// Variables locales

	// Cargo parámetros de leds
	configASSERT ( ERROR_LED != (LedRojoEnPlaca = TareaLeds_InicializarLed ( U_LED_ROJO_EP )) );
	configASSERT ( TareaLeds_ModoLed ( LedRojoEnPlaca, SUSPENSION ) );

	configASSERT ( ERROR_LED != (LedAzulEnPlaca = TareaLeds_InicializarLed ( U_LED_AZUL_EP )) );
	configASSERT ( TareaLeds_ModoLed ( LedAzulEnPlaca, BALIZA ) );

	//uLedActualizar( LedRojoEnPlaca );
	//uHALgpioEscribir (U_LED_ROJO_EP, true);
	// Acá podría haber más leds, pero no por ahora...

	// Terminada la inicialización...
	return true;
}

/*-------------------------------------------------------------------------------------------------
 * @brief  Tarea que actualiza el estado de los leds
 * @param	Ninguno
 */
void TareaTest( void *pvParameters )
{
	/* Imprimir la tarea inicializada: */
	char *pcTaskName = (char *) pcTaskGetName( NULL );
	vPrintTwoStrings( pcTaskName, " esta ejecutandose." );

	/* Enciendo leds: */
	configASSERT ( TareaLeds_EncenderLed (LedRojoEnPlaca) );
	configASSERT ( TareaLeds_EncenderLed (LedAzulEnPlaca) );

	/* Como la mayoría de las tareas, ciclo infinito... */
	for( ;; )
	{
		vTaskDelay( DELTAT_TEST );
		configASSERT ( TareaLeds_ModoLed ( LedAzulEnPlaca, PLENO ) );
		configASSERT ( TareaLeds_EncenderLed ( LedRojoEnPlaca ) );
		vTaskDelay( DELTAT_TEST );
		configASSERT ( TareaLeds_ModoLed ( LedAzulEnPlaca, INTENSIDAD10 ) );
		vTaskDelay( DELTAT_TEST );
		configASSERT ( TareaLeds_ModoLed ( LedAzulEnPlaca, TITILANTE ) );
		vTaskDelay( DELTAT_TEST );
		configASSERT ( TareaLeds_ModoLed ( LedAzulEnPlaca, TITILANTE_LENTO ) );
		vTaskDelay( DELTAT_TEST );
		configASSERT ( TareaLeds_ModoLed ( LedAzulEnPlaca, BALIZA ) );
		configASSERT ( TareaLeds_ApagarLed ( LedRojoEnPlaca ) );
	}
}

/****************************************************************** FIN DE ARCHIVO ***************/
