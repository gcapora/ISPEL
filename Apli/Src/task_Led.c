/*
 *
 *
 */


// ------ Includes -------------------------------------------------

/* Standard includes. */
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <stdbool.h>

/* Project includes. */
#include "task_Led.h"

/* Debug includes. */
#include "supporting_Functions.h"

// ------ Macros and definitions ---------------------------------------

// ------ internal data declaration ------------------------------------

// ------ internal functions declaration -------------------------------

// ------ internal data definition -------------------------------------

#define		ledTickCntMAX		pdMS_TO_TICKS( 500UL )

LDX_Config_t	LDX_Config[TL_CANTIDAD_LEDS] = {0};
	/* Así estaba definido antes:
	LDX_Config_t	LDX_Config[] = { { LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET, Blinking, 0, NULL },
						  	   	   { LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET, NotBlinking, 0, NULL},
							   	   { LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET, Blinking, 0, NULL } };
	Ahora se inicializa dentro de vTaskLedInicializar() */

// ------ external data definition -------------------------------------

// ------ internal functions definition --------------------------------

// ------ external functions definition --------------------------------

bool vTaskLedInicializar (void) {

	// Cargo parámetros de leds
	LDX_Config[TL_LED_VERDE_EP] = (LDX_Config_t) { U_LED_VERDE_EP,
												   //LD1_GPIO_Port, LD1_Pin,
		                                           GPIO_PIN_SET, Blinking, 0 };
	LDX_Config[TL_LED_AZUL_EP]  = (LDX_Config_t) { U_LED_AZUL_EP,
												   //LD2_GPIO_Port, LD2_Pin,
												   GPIO_PIN_RESET, Blinking, 0 };
	//LDX_Config[TL_LED_ROJO_EP]  = (LDX_Config_t) { U_LED_ROJO_EP,
												   //LD3_GPIO_Port, LD3_Pin,
	//											   GPIO_PIN_RESET, Blinking, 0 };

	// Inicializo todos los leds señalados en task_Led.h
	hal_pin_config_s Configuracion_Led = {0};
	Configuracion_Led.Modo = U_GPIO_MODO_SALIDA;
	Configuracion_Led.Tirar = U_GPIO_NO_TIRAR;
	Configuracion_Led.Velocidad = U_GPIO_VELOCIDAD_BAJA;

	for (uint8_t i=0; i<(TL_CANTIDAD_LEDS); i++) {
		uHALgpioInicializar ( LDX_Config[i].PIN , &Configuracion_Led );
		uHALgpioEscribir    ( LDX_Config[i].PIN , LDX_Config[i].ledState );
	}

	// Terminada la inicialización...
	vPrintString( "Modulo Task Led inicializado.\r\n" );
	return true;
}

/*------------------------------------------------------------------*/
/* Task Led thread */
void vTaskLed( void *pvParameters )
{

	/*  Declaración e inicialización de variables de la tarea */
	LDX_Config_t * ptr = (LDX_Config_t *)pvParameters;    // Nro de led pasado por argumento
	TickType_t xLastWakeTime = xTaskGetTickCount();       // Tiempo inicial de referencia
	   // La variable xLastWakeTime necesita ser inicializada con la cuenta Tick actual
	//bool EstadoLed = false;

	/* Print out the name of this task. */
	char *pcTaskName = (char *) pcTaskGetName( NULL );
	vPrintTwoStrings( pcTaskName, " esta ejecutandose." );

	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Check Led Flag */
		if( ptr->ledFlag == Blinking )
		{
			/* Invierto estado del led */
			uHALgpioInvertir ( ptr->PIN );
		   	//EstadoLed = uHALgpioLeer (ptr->PIN);
		   	//uHALgpioEscribir ( ptr->PIN, !EstadoLed );
		    //HAL_GPIO_WritePin( ptr->LDX_GPIO_Port, ptr->LDX_Pin, ptr->ledState );
		}

		/* We want this task to execute exactly every N milliseconds. */
		vPrintString( "Led X invertido.\r\n" );
		vTaskDelayUntil( &xLastWakeTime, ledTickCntMAX );
	}
}

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
