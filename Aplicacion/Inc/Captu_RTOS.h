/**************************************************************************************************
* @file		Capturadora_RTOS.h
* @author	Guillermo Caporaletti
* @brief
* @date
*
**************************************************************************************************/

#ifndef _CAPTURADORA_RTOS_H
#define _CAPTURADORA_RTOS_H

#ifdef __cplusplus
 extern "C" {
#endif

/****** Librerías (includes) *********************************************************************/

#include "stdbool.h"
#include "apli.h"
#include "uCapturadora.h"

/****** Definiciones públicas (macros) ***********************************************************/


/****** Definiciones públicas de tipos de datos (public typedef) *********************************/


/****** Declaraciones de datos externos **********************************************************/


/****** Declaración de funciones públicas ********************************************************/

bool 		CapturadoraRTOS_Inicializar 		( void );
void		Tarea_Capturadora						( void * );

/*led_id_t LedsRTOS_InicializarLed		( hal_pin_id_t );
bool 		LedsRTOS_ModoLed 				( led_id_t, led_modo_t );
bool		LedsRTOS_EncenderLed 		( led_id_t );
bool		LedsRTOS_ApagarLed 			( led_id_t );
bool		LedsRTOS_InvertirLed 		( led_id_t );
bool 		LedsRTOS_LedEncendido 		( led_id_t );
bool 		LedsRTOS_ActualizarTodos	( TickType_t );*/

/*************************************************************************************************/
#ifdef __cplusplus
}
#endif
#endif /* __TAREA_LEDS_H */
/****************************************************************** FIN DE ARCHIVO ***************/
