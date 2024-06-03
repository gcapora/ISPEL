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

	// Iniciación de capas y hardware --------------------------------------------------------------
	uoInicializar();							// Capa OSAL
	uHALinicializar();						// Capa HAL
	uHALmapInicializar( UHAL_MAP_PE5 );	// MAP=PWM en pin PE5 a 10kHz
	uHALmapConfigurarFrecuencia( UHAL_MAP_PE5 , 10000 );
	uHALmapEncender( UHAL_MAP_PE5 );

	// Inicialización de módulos -------------------------------------------------------------------

	configASSERT( true == TareaLedsInicializar()    );
	configASSERT( true == TareaBotonesInicializar() );
	configASSERT( true == TareaTestInicializar()    );

	// Escribimos mensaje de iniciación de la aplicación...
	uoEscribirTxt ( "\r\n");
	uoEscribirTxt ( Barra );
	uoEscribirTxt ( pcTextForMain );
	uoEscribirTxt ( Barra );

	uoEscribirTxtUintTxt ( "Frecuencia de senial cuadrada\t= ",
			  	  	  	  	  	  (uint32_t) round( uHALmapObtenerFrecuencia(UHAL_MAP_PE5)),
								  " Hz. \n\r");

  	uoLedEncender (UOSAL_PIN_LED_VERDE_INCORPORADO);
  	uoLedEncender (UOSAL_PIN_LED_AZUL_INCORPORADO);
  	uoLedEncender (UOSAL_PIN_LED_ROJO_INCORPORADO);

  	uoEscribirTxt ( "Espero 3 segundos con leds incorporados encendidos...\r\n");
  	uoEsperarMilis (3000);

	// Creación de las tareas ---------------------------------------------------------------------

	/* Tarea Leds en prioridad 1 */
	ret = xTaskCreate(	TareaLeds,								// Puntero a la función-tarea.
								"Tarea LEDS",							// Nombre de tarea. Para desarrollo.
								(2 * configMINIMAL_STACK_SIZE),	// Tamaño de stack en palabras.
								NULL,    								// Parametros de la tarea, que no tiene acá
								TAREA_PRIORIDAD_BAJA,				// Prioridad baja.
								&TareaLeds_m );						// Variable de administración de tarea.
	configASSERT( ret == pdPASS );

	/* Tarea Botones en prioridad 1 */
	ret = xTaskCreate( 	TareaBotones,
								"Tarea BOTONES",
								(2 * configMINIMAL_STACK_SIZE),
								NULL,
								TAREA_PRIORIDAD_MEDIA,
								&TareaBotones_m );
	configASSERT( ret == pdPASS );

	/* Tarea Test 1 */
	ret = xTaskCreate( 	TareaTest_1,
								"Tarea TEST 1",
								(2 * configMINIMAL_STACK_SIZE),
								NULL,
								TAREA_PRIORIDAD_BAJA,
								&TareaTest1_m );
	configASSERT( ret == pdPASS );

	/* Tarea Test 2 */
	ret = xTaskCreate( 	TareaTest_2,
								"Tarea TEST 2",
								(2 * configMINIMAL_STACK_SIZE),
								NULL,
								TAREA_PRIORIDAD_MEDIA,
								&TareaTest2_m );
	configASSERT( ret == pdPASS );

}

/*---------------------------------------------------------------------
  FIN DE ARCHIVO
----------------------------------------------------------------------*/
