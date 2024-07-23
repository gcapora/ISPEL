/**************************************************************************************************
* @file		Gen_RTOS.h
* @author	Guillermo Caporaletti
* @brief
* @date
**************************************************************************************************/

#ifndef _GENERADOR_RTOS_H
#define _GENERADOR_RTOS_H

#ifdef __cplusplus
 extern "C" {
#endif

/****** Librerías (includes) *********************************************************************/

#include "uGenerador.h"
#include "apli.h"

/****** Definiciones públicas (macros) ***********************************************************/


/****** Definiciones públicas de tipos de datos (public typedef) *********************************/


/****** Declaraciones de datos externos **********************************************************/


/****** Declaración de funciones públicas ********************************************************/

bool 	GenRTOS_Inicializar					( void );
bool	GenRTOS_Configurar					( gen_id_e, gen_conf_s *, TickType_t );
bool	GenRTOS_Obtener						( gen_id_e, gen_conf_s *, TickType_t );
bool	GenRTOS_Encender						( gen_id_e, TickType_t );
bool	GenRTOS_Apagar							( gen_id_e, TickType_t );
bool	GenRTOS_EstaEncendido				( gen_id_e, TickType_t );
bool	GenRTOS_EscribirConfiguraciones	( TickType_t );
//void	Tarea_Capturadora						( void * );

/*************************************************************************************************/
#ifdef __cplusplus
}
#endif
#endif /* _GENERADOR_RTOS_H */
/****************************************************************** FIN DE ARCHIVO ***************/
