/**************************************************************************************************
* @file		tarea_test_1.c
* @author	Guillermo Caporaletti
* @brief	Tarea para testear.
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
	configASSERT ( ERROR_LED != (LedRojoEnPlaca = uLedInicializar ( U_LED_ROJO_EP )) );
	configASSERT ( true == TareaLedsModo ( LedRojoEnPlaca, SUSPENSION ) );

	configASSERT ( ERROR_LED != (LedAzulEnPlaca = uLedInicializar ( U_LED_AZUL_EP )) );
	configASSERT ( true == TareaLedsModo ( LedAzulEnPlaca, BALIZA ) );

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
	configASSERT ( true == uLedEncender (LedRojoEnPlaca) );
	configASSERT ( true == uLedEncender (LedAzulEnPlaca) );

	/* Como la mayoría de las tareas, ciclo infinito... */
	for( ;; )
	{
		vTaskDelay( DELTAT_TEST );
		configASSERT ( true == TareaLedsModo ( LedAzulEnPlaca, PLENO ) );
		vTaskDelay( DELTAT_TEST );
		configASSERT ( true == TareaLedsModo ( LedAzulEnPlaca, INTENSIDAD10 ) );
		vTaskDelay( DELTAT_TEST );
		configASSERT ( true == TareaLedsModo ( LedAzulEnPlaca, TITILANTE ) );
		vTaskDelay( DELTAT_TEST );
		configASSERT ( true == TareaLedsModo ( LedAzulEnPlaca, TITILANTE_LENTO ) );
		vTaskDelay( DELTAT_TEST );
		configASSERT ( true == TareaLedsModo ( LedAzulEnPlaca, BALIZA ) );
	}
}

/****************************************************************** FIN DE ARCHIVO ***************/