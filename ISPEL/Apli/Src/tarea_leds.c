/**************************************************************************************************
* @file		tarea_leds.c
* @author	Guillermo Caporaletti
* @brief	Tarea que controla todos los leds.
* @date		julio de 2023
*
**************************************************************************************************/


/****** Librerías (includes) *********************************************************************/

#include "tarea_leds.h"

/****** Definiciones privadas (macros) ***********************************************************/

//#define		ledTickCntMAX		pdMS_TO_TICKS( 500UL )
#define		LED_DELTAT			pdMS_TO_TICKS( 1UL )

/****** Definiciones privadas de tipos (private typedef) *****************************************/


/****** Definición de datos privados *************************************************************/

/* Define the strings that will be passed in as the Supporting Functions parameters.
 * These are defined const and off the stack to ensure they remain valid when the
 * tasks are executing. */
//const char *pcTextForTask_LDXTOn		= " - LDX turn On \r\n";
//const char *pcTextForTask_LDXTOff		= " - LDX turn Off\r\n";

/****** Definición de datos públicos *************************************************************/

led_id_t LedRojoEnPlaca, LedAzulEnPlaca;

/****** Declaración de funciones privadas ********************************************************/


/****** Definición de funciones privadas *********************************************************/


/****** Definición de funciones públicas *********************************************************/

/*-------------------------------------------------------------------------------------------------
 * @brief  Inicializa todos los leds a utilizar
 * @param	Ninguno
 * @retval true, si no hubo problemas
 */
bool TareaLedsInicializar (void) {

	// Variables locales
	//led_modo_t configuracionLed = {0};

	// Cargo parámetros de leds
	if ( ERROR_LED == (LedRojoEnPlaca = uLedInicializar ( U_LED_ROJO_EP )) ) while(1) { /* ERROR */ };
	if ( false == uLedConfigurarModo ( LedRojoEnPlaca, SUSPENSION ) ) while(1) { /* ERROR */ };

	if ( ERROR_LED == (LedAzulEnPlaca = uLedInicializar ( U_LED_AZUL_EP )) ) while(1) { /* ERROR */ };
	if ( false == uLedConfigurarModo ( LedAzulEnPlaca, BALIZA ) ) while(1) { /* ERROR */ };

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
void TareaLeds( void *pvParameters )
{

	/* Variables locales de la tarea */
		// Por ahora no tengo parámetros y no necesito hacer:
		// ptr = (LDX_Config_t *)pvParameters;
	//TickType_t xLastWakeTime = xTaskGetTickCount();       // Tiempo inicial de referencia
		// La variable xLastWakeTime necesita ser inicializada con la cuenta Tick actual
	// bool EstadoLed = false;

	/* Si deseo imprimir la tarea inicializada: */
	//char *pcTaskName = (char *) pcTaskGetName( NULL );
	//vPrintTwoStrings( pcTaskName, "   - is running\r\n" );

	if (false == uLedEncender (LedRojoEnPlaca) ) while (1) { /* ERROR */ };
	if (false == uLedEncender (LedAzulEnPlaca) ) while (1) { /* ERROR */ };

	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		uLedActualizar( LedRojoEnPlaca );
		uLedActualizar( LedAzulEnPlaca );
		vTaskDelay( LED_DELTAT );
	}
}

/****************************************************************** FIN DE ARCHIVO ***************/
