/*
 *
 */

#ifndef __TASK_LED_H
#define __TASK_LED_H

#ifdef __cplusplus
 extern "C" {
#endif

// ------ inclusions ---------------------------------------------------
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"

// ------ macros -------------------------------------------------------

// ------ typedef ------------------------------------------------------

// ------ external data declaration ------------------------------------
typedef enum	ledFlag_e{ Blinking, NotBlinking } ledFlag_t;

typedef struct
{
	GPIO_TypeDef*	LDX_GPIO_Port;
	uint16_t		LDX_Pin;
	GPIO_PinState	ledState;
	ledFlag_t		ledFlag;
	TickType_t 		ledTickCnt;
} LDX_Config_t;

extern LDX_Config_t LDX_Config[];

// ------ external functions declaration -------------------------------

void vTaskLed( void *pvParameters );

#ifdef __cplusplus
}
#endif

#endif /* __TASK_LED_H */

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
