/**************************************************************************************************
* @file    uleds.h
* @author  Guillermo Caporaletti
* @brief   Módulo para manejo de leds
* @date	   julio de 2023
*
**************************************************************************************************/

#ifndef __ISPEL_ULEDS_H
#define __ISPEL_ULEDS_H

/****** Librerías (includes) *********************************************************************/

#include "uOSAL.h"
#include "uHAL.h"

/****** Definiciones públicas (macros) ***********************************************************/

#define CANTIDAD_LED    10
#define ERROR_LED       -1

/****** Definiciones públicas de tipos de datos (public typedef) *********************************/

typedef int8_t	led_id_t;
typedef enum {
	 LED_APAGADO,
	 LED_ENCENDIDO
}				led_estado_t;
typedef enum {
	PLENO,
	PREDETERMINADO = PLENO,
	INTENSIDAD100 = PLENO,
	INTENSIDAD60,
	INTENSIDAD30,
	INTENSIDAD10,
	INTENSIDAD05,
	TITILANTE,
	TITILANTE_LENTO,
	BALIZA,
	SUSPENSION,
	CANTIDAD_MODOS_LED,
	ERROR_MODO_LED = CANTIDAD_MODOS_LED,
} 				led_modo_t;

/****** Declaraciones de datos externos **********************************************************/

/****** Declaración de funciones públicas ********************************************************/

led_id_t	uLedInicializar (hal_pin_id_t);
bool		uLedEncender (led_id_t);
bool		uLedApagar (led_id_t);
bool		uLedActualizar (led_id_t);
bool		uLedActualizarTodo ( void );
bool		uLedConfigurarModo (led_id_t, led_modo_t);
led_estado_t uLedObtenerEstado (led_id_t);
led_estado_t uLedObtenerEstadoFisico (led_id_t);
led_modo_t	uLedObtenerModo (led_id_t);
bool		uLedDesinicializar (led_id_t);

/*************************************************************************************************/
#endif /* __ISPEL_ULED_H */
/****************************************************************** FIN DE ARCHIVO ***************/
