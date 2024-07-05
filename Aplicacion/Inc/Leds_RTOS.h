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

#include "uLeds.h"
#include "apli.h"

/****** Definiciones públicas (macros) ***********************************************************/

#define TLS_CANTIDAD_LEDS	3
#define TLS_LED_VERDE_EP	0
#define TLS_LED_AZUL_EP		1
#define TLS_LED_ROJO_EP		2

/****** Definiciones públicas de tipos de datos (public typedef) *********************************/


/****** Declaraciones de datos externos **********************************************************/


/****** Declaración de funciones públicas ********************************************************/

bool 		LedsRTOS_Inicializar 		( void );
led_id_t LedsRTOS_InicializarLed		( hal_pin_id_t );
bool 		LedsRTOS_ModoLed 				( led_id_t, led_modo_t );
bool		LedsRTOS_EncenderLed 		( led_id_t );
bool		LedsRTOS_ApagarLed 			( led_id_t );
bool		LedsRTOS_InvertirLed 		( led_id_t );
bool 		LedsRTOS_LedEncendido 		( led_id_t );
bool 		LedsRTOS_ActualizarTodos	( TickType_t );

/*************************************************************************************************/
#ifdef __cplusplus
}
#endif
#endif /* __TAREA_LEDS_H */
/****************************************************************** FIN DE ARCHIVO ***************/
