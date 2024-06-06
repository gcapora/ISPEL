/**************************************************************************************************
* @file		tarea_leds.h
* @author	Guillermo Caporaletti
* @brief		Control de botones en RTOS.
* @date		agosto de 2023
*
**************************************************************************************************/

#ifndef __TAREA_BOTONES_H
#define __TAREA_BOTONES_H

#ifdef __cplusplus
 extern "C" {
#endif

/****** Librerías (includes) *********************************************************************/

#include "stdbool.h"		// Esta librería es usada por la función TareaLedsInicializar
#include "apli.h"			// Aquí están declarados los recursos compartidos
#include <uBotones.h>	// Módulo específico de utilización de botones

/****** Definiciones públicas (macros) ***********************************************************/


/****** Definiciones públicas de tipos de datos (public typedef) *********************************/


/****** Declaraciones de datos externos **********************************************************/


/****** Declaración de funciones públicas ********************************************************/

bool 			BotonesRTOS_Inicializar 				( void );
boton_id_t	BotonesRTOS_InicializarBoton 			( hal_pin_id_t );
bool			BotonesRTOS_ActualizarTodo 			( TickType_t );
bool 			BotonesRTOS_BotonFlancoPresionado	( boton_id_t );
bool 			BotonesRTOS_BotonPresionadoLargo		( boton_id_t );

/*************************************************************************************************/
#ifdef __cplusplus
}
#endif
#endif /* __TAREA_BOTONESS_H */
/****************************************************************** FIN DE ARCHIVO ***************/
