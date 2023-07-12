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
#include "stdbool.h"
#include "uHAL.h"

// ------ macros -------------------------------------------------------
#define TL_CANTIDAD_LEDS	2
#define TL_LED_VERDE_EP		0
#define TL_LED_AZUL_EP		1
#define TL_LED_ROJO_EP		2

// ------ typedef ------------------------------------------------------

// ------ external data declaration ------------------------------------
typedef enum	ledFlag_e{ Blinking, NotBlinking } ledFlag_t;

typedef struct
{
	hal_pin_id_t	PIN;
//	GPIO_TypeDef*	LDX_GPIO_Port;
//	uint16_t		LDX_Pin;
	GPIO_PinState	ledState;
	ledFlag_t		ledFlag;
	TickType_t 		ledTickCnt;
} LDX_Config_t;

extern LDX_Config_t LDX_Config[];

// ------ external functions declaration -------------------------------

bool vTaskLedInicializar (void);
void vTaskLed( void *pvParameters );

#ifdef __cplusplus
}
#endif

#endif /* __TASK_LED_H */

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
