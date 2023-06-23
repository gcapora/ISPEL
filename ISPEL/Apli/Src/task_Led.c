/*
 *
 *
 */


// ------ Includes -------------------------------------------------
/* Project includes. */
#include "../../Apli/Inc/task_Led.h"

#include "main.h"
#include "cmsis_os.h"

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Demo includes. */
//#include "supporting_Functions.h"

/* Application includes. */
//#include "app_Resources.h"

// ------ Macros and definitions ---------------------------------------

// ------ internal data declaration ------------------------------------

// ------ internal functions declaration -------------------------------

// ------ internal data definition -------------------------------------
/* Define the strings that will be passed in as the Supporting Functions parameters.
 * These are defined const and off the stack to ensure they remain valid when the
 * tasks are executing. */
const char *pcTextForTask_LDXTOn		= " - LDX turn On \r\n";
const char *pcTextForTask_LDXTOff		= " - LDX turn Off\r\n";

#define			ledTickCntMAX		pdMS_TO_TICKS( 125UL )

LDX_Config_t	LDX_Config[] 	= { { LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET, NotBlinking, 0 },
							  	    { LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET, Blinking, 0 },
									{ LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET, Blinking, 0 } };

// ------ external data definition -------------------------------------

// ------ internal functions definition --------------------------------

// ------ external functions definition --------------------------------

/*------------------------------------------------------------------*/
/* Task Led thread */
void vTaskLed( void *pvParameters )
{
	/*  Declaración e inicialización de variables de la tarea */
	LDX_Config_t * ptr = (LDX_Config_t *)pvParameters;    // Nro de led pasado por argumento
	TickType_t xLastWakeTime = xTaskGetTickCount();       // Tiempo inicial de referencia
	   // La variable xLastWakeTime necesita ser inicializada con la cuenta Tick actual

	//char *pcTaskName = (char *) pcTaskGetName( NULL );
	/* Print out the name of this task. */
	// vPrintTwoStrings( pcTaskName, "   - is running\r\n" );

	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Check Led Flag */
		if( ptr->ledFlag == Blinking )
		{
			/* Check, Update and Print Led State */
		   	if( ptr->ledState == GPIO_PIN_RESET )
		   	{
		   		ptr->ledState = GPIO_PIN_SET;
            	//vPrintTwoStrings( pcTaskName, pcTextForTask_LDXTOn );
		   	}
	    	else
	    	{
	    		ptr->ledState = GPIO_PIN_RESET;
            	//vPrintTwoStrings( pcTaskName, pcTextForTask_LDXTOff );
		   	}
			/* Update HW Led State */
		   	HAL_GPIO_WritePin( ptr->LDX_GPIO_Port, ptr->LDX_Pin, ptr->ledState );
		}

		/* We want this task to execute exactly every 250 milliseconds. */
		vTaskDelayUntil( &xLastWakeTime, ledTickCntMAX );
	}
}

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
