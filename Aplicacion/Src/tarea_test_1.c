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

#define DELTAT_TEST			pdMS_TO_TICKS( 2000UL )

/****** Definiciones privadas de tipos (private typedef) *****************************************/


/****** Definición de datos privados *************************************************************/

led_id_t    LedVerdeEnPlaca, LedRojoEnPlaca, LedAzulEnPlaca;
boton_id_t  BotonEnPlaca;

/****** Definición de datos públicos *************************************************************/


/****** Declaración de funciones privadas ********************************************************/


/****** Definición de funciones privadas *********************************************************/


/****** Definición de funciones públicas *********************************************************/

/*-------------------------------------------------------------------------------------------------
 * @brief  Inicializa todos los leds a utilizar
 * @param	Ninguno
 * @retval true, si no hubo problemas
 */
bool TareaTestInicializar (void)
{
	// Enciendo DAC
	uint32_t senial[32] = {3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000,
			                     3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000,
								 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	uHALdacdmaConfigurar(UHAL_DAC_1, NULL);
   uHALdacdmaComenzar ( UHAL_DAC_1, senial , 32 );

	// Cargo parámetros de leds
	configASSERT ( ERROR_LED != (LedRojoEnPlaca = TareaLeds_InicializarLed ( U_LED_ROJO_EP )) );
	configASSERT ( TareaLeds_ModoLed ( LedRojoEnPlaca, SUSPENSION ) );

	configASSERT ( ERROR_LED != (LedAzulEnPlaca  = TareaLeds_InicializarLed ( U_LED_AZUL_EP )) );
	configASSERT ( TareaLeds_ModoLed ( LedAzulEnPlaca, BALIZA ) );

	configASSERT ( ERROR_LED != (LedVerdeEnPlaca = TareaLeds_InicializarLed ( U_LED_VERDE_EP )) );

	// Inicializo boton
	configASSERT ( ERROR_BOTON != (BotonEnPlaca = TareaBotones_InicializarBoton ( U_BOTON_EP )) );

	// Terminada la inicialización...
	return true;
}

/*-------------------------------------------------------------------------------------------------
 * @brief  Tarea que actualiza el estado de los leds
 * @param	Ninguno
 */
void TareaTest_1( void *pvParameters )
{
	uint32_t tiempo1, tiempo2;

	/* Imprimir la tarea inicializada: */
	char *pcTaskName = (char *) pcTaskGetName( NULL );
	uoEscribirTxtTxt ( pcTaskName, " esta ejecutandose.\n\r" );
	// vPrintStringAndNumber ("Cuenta actual: ", xTaskGetTickCount());
  	// vPrintStringAndNumber ("Cuenta micro: ", uMicrosegundos());

  	//vPrintString (" us.\n\r");

	/* Enciendo leds: */
	configASSERT ( TareaLeds_EncenderLed (LedRojoEnPlaca)  );
	configASSERT ( TareaLeds_EncenderLed (LedAzulEnPlaca)  );
	configASSERT ( TareaLeds_EncenderLed (LedVerdeEnPlaca) );

  	tiempo2 = uoMicrosegundos();
  	tiempo1 = uoMilisegundos();
  	while ( (uoMilisegundos()-tiempo1) < 1000) {
  	  		// NADA!!!
  	}
  	uoEscribirTxtUintTxt ("Microsegundos tras 1000 milisegundos: ", uoMicrosegundos()-tiempo2, "\n\r");

  	tiempo2 = uoMicrosegundos();
  	tiempo1 = uoMilisegundos();
  	while ( (uoMicrosegundos()-tiempo2) < 1000) {
  	  		// NADA!!!
  	}
  	uoEscribirTxtUintTxt ("Milisegundos tras 1000 microsegundos: ", uoMilisegundos()-tiempo1, "\n\r");

	/* Como la mayoría de las tareas, ciclo infinito... */
	for( ;; )
	{
		vTaskDelay( DELTAT_TEST );
		configASSERT ( TareaLeds_ModoLed ( LedAzulEnPlaca, PLENO ) );
		//vPrintStringAndNumber ("Tiempo ", xTaskGetTickCount());

		vTaskDelay( DELTAT_TEST );
		configASSERT ( TareaLeds_ModoLed ( LedAzulEnPlaca, TITILANTE ) );
		//vPrintStringAndNumber ("Tiempo ", xTaskGetTickCount());

		vTaskDelay( DELTAT_TEST );
		configASSERT ( TareaLeds_ModoLed ( LedAzulEnPlaca, TITILANTE_LENTO ) );
	  	//vPrintStringAndNumber ("Tiempo ", xTaskGetTickCount());
	}
}

/*-------------------------------------------------------------------------------------------------
 * @brief  Tarea que actualiza el estado de los leds
 * @param	Ninguno
 */
void TareaTest_2( void *pvParameters )
{
	/* Imprimir la tarea inicializada: */
	char *pcTaskName = (char *) pcTaskGetName( NULL );
	uoEscribirTxtTxt ( pcTaskName, " esta ejecutandose.\n\r" );

	/* Como la mayoría de las tareas, ciclo infinito... */
	for( ;; )
	{
		vTaskDelay( 50UL );
		if ( true == TareaBotones_BotonFlancoPresionado ( BotonEnPlaca ) ){
			// Cambio estado del led...
			TareaLeds_InvertirLed (LedVerdeEnPlaca);
		}
	}
}

/****************************************************************** FIN DE ARCHIVO ***************/
