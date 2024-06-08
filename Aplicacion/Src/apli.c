/**************************************************************************************************
* @file		apli.c
* @author	Guillermo Caporaletti
* @brief	Módulo principal de la aplicación ISPEL
* @date		julio de 2023
*
**************************************************************************************************/

/****** Librerías (includes) *********************************************************************/

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

/* Project includes. */
#include <apli.h>

/****** Definiciones privadas (macros) ***********************************************************/

#define TAREA_PRIORIDAD_BAJA     ( tskIDLE_PRIORITY + 1UL )
#define TAREA_PRIORIDAD_MEDIA    ( tskIDLE_PRIORITY + 2UL )
#define TAREA_PRIORIDAD_ALTA     ( tskIDLE_PRIORITY + 3UL )

/****** Definiciones privadas de tipos (private typedef) *****************************************/


/****** Definición de datos privados *************************************************************/

/* Variables usadas para referencias las tareas. */
TaskHandle_t TareaLeds_m;
TaskHandle_t TareaBotones_m;
TaskHandle_t TareaTest1_m, TareaTest2_m;

/* Mensajes... */
const char *pcTextForMain = "Test ISPEL en ejecucion\r\n";
const char *Barra         = "========================================\r\n";

/****** Definición de datos públicos *************************************************************/

boton_id_t  BotonEnPlaca;

/****** Declaración de funciones privadas ********************************************************/


/****** Definición de funciones privadas *********************************************************/


/****** Definición de funciones públicas *********************************************************/

/*-------------------------------------------------------------------------------------------------
 * @brief	Inicializa el módulo principal de APLICACION
 * @param	Ninguno
 * @retval	Ninguno
 */
void apliInicializar( void )
{
	// Variables locales
	BaseType_t ret = pdFAIL;
  	// void* ptr = NULL;

	// Inicialización de capas de abstracción uHAL y uOSAL -----------------------------------------

	uoInicializar();							// Capa OSAL
	uHALinicializar();						// Capa HAL
	uHALmapInicializar( UHAL_MAP_PE5 ); // Objeto señal cuadrada MAP=PWM en pin PE5

	// Inicialización de módulos, tareas y objetos--------------------------------------------------

	configASSERT( true == CaptuRTOS_Inicializar() );
	configASSERT( true == LedsRTOS_Inicializar()    );
	configASSERT( true == BotonesRTOS_Inicializar() );

	configASSERT( true == TareaTestInicializar()    );

	configASSERT ( ERROR_BOTON != (BotonEnPlaca = BotonesRTOS_InicializarBoton ( U_BOTON_EP )) );

	// Mensaje de inicio de APLICACION -------------------------------------------------------------

	uoEscribirTxt ( "\r\n");
	uoEscribirTxt ( Barra );
	uoEscribirTxt ( pcTextForMain );
	uoEscribirTxt ( Barra );

	// Configuramos señal testigo a FREC_TESTIGO

	uHALmapConfigurarFrecuencia( UHAL_MAP_PE5 , FREC_TESTIGO );
	uHALmapEncender( UHAL_MAP_PE5 );
	uoEscribirTxtUintTxt ( "Frecuencia de senial cuadrada\t= ",
			  	  	  	  	  	  (uint32_t) round( uHALmapObtenerFrecuencia(UHAL_MAP_PE5)),
								  " Hz. \n\r");

	// Probamos leds controlados por UOSAL

  	uoLedEncender	(UOSAL_PIN_LED_VERDE_INCORPORADO);
  	uoLedEncender	(UOSAL_PIN_LED_AZUL_INCORPORADO);
  	uoLedEncender	(UOSAL_PIN_LED_ROJO_INCORPORADO);

  	uoEscribirTxt	( "Espero 1 segundo con leds incorporados encendidos...\r\n");
  	uoEsperarMilis (1000);
  	uoLedApagar		(UOSAL_PIN_LED_VERDE_INCORPORADO);

	// Creación de las tareas ---------------------------------------------------------------------

	/* Tarea Leds en prioridad 1 */
	ret = xTaskCreate(	Tarea_PALTA_1ms,						// Puntero a la función-tarea.
								"PALTA_1ms",							// Nombre de tarea. Para desarrollo.
								(2 * configMINIMAL_STACK_SIZE),	// Tamaño de stack en palabras.
								NULL,    								// Parametros de la tarea, que no tiene acá
								TAREA_PRIORIDAD_ALTA,				// Prioridad alta.
								&TareaLeds_m );						// Variable de administración de tarea.
	configASSERT( ret == pdPASS );

	/* Tarea Botones en prioridad 1 */
	ret = xTaskCreate( 	Tarea_PMEDIA_10ms,
								"PMEDIA_10ms",
								(2 * configMINIMAL_STACK_SIZE),
								NULL,
								TAREA_PRIORIDAD_MEDIA,
								&TareaBotones_m );
	configASSERT( ret == pdPASS );

	/* Tarea Test 1 */
	ret = xTaskCreate( 	TareaTest_1,
								"TEST 1",
								(2 * configMINIMAL_STACK_SIZE),
								NULL,
								TAREA_PRIORIDAD_MEDIA,
								&TareaTest1_m );
	configASSERT( ret == pdPASS );

	/* Tarea Test 2 */
	ret = xTaskCreate( 	Tarea_Capturadora,
								"CAPTURADORA",
								(2 * configMINIMAL_STACK_SIZE),
								NULL,
								TAREA_PRIORIDAD_BAJA,
								&TareaTest2_m );
	configASSERT( ret == pdPASS );

}

/*-------------------------------------------------------------------------------------------------
 * @brief	Tarea que se actualiza con ALTA RPIORIDAD cada 1 ms
 * @param	Ninguno
 * @descripcion:
 * 	Incluye:
 * 	- Lectura de UART (solo carga, no procesa).
 * 	- Actualización de leds.
 */
void Tarea_PALTA_1ms( void *pvParameters )
{
	/* Variables locales */
	char *pcTaskName = (char *) pcTaskGetName( NULL );
	char Lectura [11] = {0};
   TickType_t Tiempo0;

	/* Imprimir la tarea iniciada */
	uoEscribirTxt3 ( "MSJ ", pcTaskName, " esta ejecutandose.\n\r" );

	/* Ciclo infinito, como la mayoría de las tareas: */
	Tiempo0 = xTaskGetTickCount();
	for( ;; )
	{

		// Verificamos lectura de UART
		if ( uoLeerTxt(Lectura,10,5)>0 ) {
			uoEscribirTxt (Lectura);
		}

		// Actualización de leds
		LedsRTOS_ActualizarTodos ( 0 );  // "0" porque prefiero saltear operación que esperarla

		// Retardo sincronizado
		vTaskDelayUntil( &Tiempo0, PERIODO_1MS );
	}
}

/*-------------------------------------------------------------------------------------------------
 * @brief	Tarea que se actualiza con MEDIA RPIORIDAD cada 10 ms
 * @param	Ninguno
 * @descripcion:
 * 	Incluye:
 * 	- Actualización de botones.
 */
void Tarea_PMEDIA_10ms ( void *pvParameters )
{
	/* Imprimir la tarea iniciada: */
	char *pcTaskName = (char *) pcTaskGetName( NULL );
	uoEscribirTxt3 ( "MSJ ", pcTaskName, " esta ejecutandose.\n\r" );

	/* Ciclo infinito, como la mayoría de las tareas: */
	for( ;; )
	{
		BotonesRTOS_ActualizarTodo ( portMAX_DELAY );
		vTaskDelay( PERIODO_10MS );  // Se supone que no es tan crítico el sincronismo.
	}
}


/*---------------------------------------------------------------------
  FIN DE ARCHIVO
----------------------------------------------------------------------*/
