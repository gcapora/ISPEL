/*---------------------------------------------------------------------
 * Interfaz de señales para prácticas educativas de laboratorio
 *
 * Año:     2023
 * Autor:   Guillermo F. Caporaletti
 * Archivo: main_app.c
 *
 *--------------------------------------------------------------------*/

// ------ Includes -----------------------------------------------------
/* Project includes. */

#include "main.h"
#include "cmsis_os.h"	// ¿Por qué había que incluir esta librería?

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Demo includes. */
// #include "supporting_Functions.h" // ¿De dónde incluimos esta librería?

/* Application & Tasks includes. */


//#include "task_Button.h"
#include "../../Apli/Inc/task_Led.h"

// ------ Macros and definitions ---------------------------------------

// ------ internal data declaration ------------------------------------
/* Declare a variable of type xTaskHandle. This is used to reference tasks. */
//TaskHandle_t xTaskButtonHandle;
TaskHandle_t xTaskLedHandle;

// ------ internal functions declaration -------------------------------

// ------ internal data definition -------------------------------------
const char *pcTextForMain = "Embebido Interfaz ejecutandose...\r\n\n";

// ------ external data definition -------------------------------------

// ------ internal functions definition --------------------------------

// ------ external functions definition --------------------------------


/*------------------------------------------------------------------*/
/* App Initialization */
void appInit( void )
{
	/* Print out the name of this Example. */
  	//vPrintString( pcTextForMain );

	// Variables utilizadas por la aplicación
	LDX_Config_t* ptr, *led2, *led3;
	BaseType_t ret;

	// Inicializo módulo task_led
	configASSERT( true == vTaskLedInicializar());

	/* Task Led thread at priority 1 */
	LDX_Config[0].PIN = HAL_PIN_PB0;
	ptr = &LDX_Config[0];
	ret = xTaskCreate( vTaskLed,						/* Pointer to the function thats implement the task. */
					   "Task Led",						/* Text name for the task. This is to facilitate debugging only. */
					   (2 * configMINIMAL_STACK_SIZE),	/* Stack depth in words. 				*/
	                   (void*)ptr,    					/* Pass the ptr as the task parameter. */
					   (tskIDLE_PRIORITY + 1UL),		/* This task will run at priority 1. 		*/
					   &xTaskLedHandle );				/* We are using a variable as task handle.	*/

	/* Check the task was created successfully. */
	configASSERT( ret == pdPASS );

	/* Task Led thread at priority 1 */
	led2 = &LDX_Config[1];
	ret = xTaskCreate( vTaskLed, "Task Led", (2 * configMINIMAL_STACK_SIZE),
			           (void*)led2, (tskIDLE_PRIORITY + 1UL), &xTaskLedHandle );
	/* Check the task was created successfully. */
	configASSERT( ret == pdPASS );

	/* Task Led thread at priority 1 */
	led3 = &LDX_Config[2];
	ret = xTaskCreate( vTaskLed, "Task Led", (2 * configMINIMAL_STACK_SIZE),
			           (void*)led3, (tskIDLE_PRIORITY + 1UL), &xTaskLedHandle );
	/* Check the task was created successfully. */
	configASSERT( ret == pdPASS );



	//ptr = &LDX_Config[0];
	/* Task Button thread at priority 1 */
	//ret = xTaskCreate( vTaskButton,					/* Pointer to the function thats implement the task. */
	//				   "Task Button",				/* Text name for the task. This is to facilitate debugging only. */
	//				   (2 * configMINIMAL_STACK_SIZE),	/* Stack depth in words. 				*/
	//                   (void*)ptr,    				/* Pass the ptr as the task parameter. */
	//				   (tskIDLE_PRIORITY + 1UL),	/* This task will run at priority 1. 		*/
	//				   &xTaskButtonHandle );		/* We are using a variable as task handle.	*/

	/* Check the task was created successfully. */
	//configASSERT( ret == pdPASS );
}

/*---------------------------------------------------------------------
  FIN DE ARCHIVO
----------------------------------------------------------------------*/
