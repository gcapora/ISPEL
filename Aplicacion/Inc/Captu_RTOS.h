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

bool 	CaptuRTOS_Inicializar				( void );
bool	CaptuRTOS_Comenzar					( TickType_t );  // Inicia una nueva captura.
bool	CaptuRTOS_Parar						( TickType_t );  // Cancela una captura en curso.

bool	CaptuRTOS_Configurar					( capturadora_config_s *, TickType_t );  // Configuración de base de tiempo y disparo.
bool	CaptuRTOS_Obtener						( capturadora_config_s *, TickType_t );
bool	CaptuRTOS_EscribirConfiguraciones( TickType_t );

bool  CaptuRTOS_EntradaConfigurar		( entrada_id_e, entrada_config_s *, TickType_t ); // Configuración de un canal.
bool  CaptuRTOS_EntradaObtener			( entrada_id_e, entrada_config_s *, TickType_t );
bool  CaptuRTOS_EntradaEncender			( entrada_id_e, TickType_t );  // Encendido y apagado de un canal.
bool  CaptuRTOS_EntradaApagar				( entrada_id_e, TickType_t );

void	Tarea_Capturadora						( void * );

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
