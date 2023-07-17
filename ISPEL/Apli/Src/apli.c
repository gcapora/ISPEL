/*---------------------------------------------------------------------
 * Interfaz de señales para prácticas educativas de laboratorio
 *
 * Año:     2023
 * Autor:   Guillermo F. Caporaletti
 * Archivo: apli.c
 *
 *--------------------------------------------------------------------*/

// ------ Includes -----------------------------------------------------

/* Standard includes. */
#include <apli.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Project includes. */
#include "task_Led.h"
#include "tarea_leds.h"

// ------ Macros and definitions ---------------------------------------


// ------ internal data declaration ------------------------------------

/* Declare a variable of type xTaskHandle. This is used to reference tasks. */
//TaskHandle_t xTaskButtonHandle;
TaskHandle_t xTaskLedHandle;

// ------ internal functions declaration -------------------------------


// ------ internal data definition -------------------------------------

const char *pcTextForMain = "Embebido ISPEL ejecutandose...\r\n";

// ------ external data definition -------------------------------------


// ------ internal functions definition --------------------------------


// ------ external functions definition --------------------------------


/*------------------------------------------------------------------*/
/* App Initialization */
void apliInicializar( void )
{
	// Iniciación de la aplicación ----------------------------------------------------------------
	// Escribimos mensaje de iniciación de la aplicación...
	vPrintString( "----------------------------------------\r\n");
  	vPrintString( pcTextForMain );

	// Variables utilizadas por la aplicación

  	LDX_Config_t* ptr;
	BaseType_t ret;

	// Inicialización de módulos ------------------------------------------------------------------

	configASSERT( true == vTaskLedInicializar());
	configASSERT( true == TareaLedsInicializar());
	configASSERT( true == TareaTestInicializar());

	// Creación de las tareas ---------------------------------------------------------------------

	/* Task LED en prioridad 1 */
	ptr = &LDX_Config[0];
	ret = xTaskCreate( vTaskLed,						/* Puntero a la función-tarea.			*/
					   "Task LED",						/* Nombre de tarea. Para desarrollo.	*/
					   (2 * configMINIMAL_STACK_SIZE),	/* Tamaño de stack en palabras.			*/
	                   (void*)ptr,    					/* Parametros de la tarea.				*/
					   (tskIDLE_PRIORITY + 1UL),		/* Prioridad 1.							*/
					   &xTaskLedHandle );				/* Variable de task handle.				*/

	/* Check the task was created successfully. */
	configASSERT( ret == pdPASS );

	/* Tarea Leds en prioridad 1 */
	ret = xTaskCreate( TareaLeds,
					   "Tarea LEDS",
					   (2 * configMINIMAL_STACK_SIZE),
					   (void*)ptr,
					   (tskIDLE_PRIORITY + 1UL),
					   &xTaskLedHandle );
	configASSERT( ret == pdPASS );

	/* Tarea Test */
	ret = xTaskCreate( TareaTest, "Tarea TEST", (2 * configMINIMAL_STACK_SIZE),
					   (void*)ptr, (tskIDLE_PRIORITY + 1UL), &xTaskLedHandle );
	configASSERT( ret == pdPASS );

}

/*---------------------------------------------------------------------
  FIN DE ARCHIVO
----------------------------------------------------------------------*/
