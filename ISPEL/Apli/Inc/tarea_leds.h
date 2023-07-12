/**************************************************************************************************
* @file		tarea_leds.h
* @author	Guillermo Caporaletti
* @brief	Tarea que controla todos los leds.
* @date		julio de 2023
*
**************************************************************************************************/

#ifndef __TAREA_LEDS_H
#define __TAREA_LEDS_H

#ifdef __cplusplus
 extern "C" {
#endif

/****** Librerías (includes) *********************************************************************/

#include "stdbool.h"	// Esta librería es usada por la tarea
#include "main_app.h"	// Aquí están declarados los recursos compartidos
#include "uHAL.h"		// Nuestra interfaz de abstracción de hardware
#include "uleds.h"		// Módulo específico de utilización de leds

/****** Definiciones públicas (macros) ***********************************************************/

#define TLS_CANTIDAD_LEDS	3
#define TLS_LED_VERDE_EP	0
#define TLS_LED_AZUL_EP		1
#define TLS_LED_ROJO_EP		2

/****** Definiciones públicas de tipos de datos (public typedef) *********************************/


/****** Declaraciones de datos externos **********************************************************/

extern led_id_t LedRojoEnPlaca;

/****** Declaración de funciones públicas ********************************************************/

bool TareaLedsInicializar (void);
void TareaLeds ( void *pvParameters );

/*************************************************************************************************/
#ifdef __cplusplus
}
#endif
#endif /* __TAREA_LEDS_H */
/****************************************************************** FIN DE ARCHIVO ***************/
