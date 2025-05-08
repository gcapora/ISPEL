/**************************************************************************************************
* @file    uBotones.h
* @author  Guillermo F. Caporaletti
* @brief   Módulo para manejo de botones con antirrebote.
* @date	   2024
* @version v1
*
**************************************************************************************************/

#ifndef __ISPEL_UBOTONES_H
#define __ISPEL_UBOTONES_H

/****** Librerías (includes) *********************************************************************/

#include "uOSAL.h"
#include "uHAL.h"

/****** Definiciones públicas (macros) ***********************************************************/

#define CANTIDAD_BOTONES  10
#define ERROR_BOTON       -1

/****** Definiciones públicas de tipos de datos (public typedef) *********************************/

typedef int8_t	boton_id_t;

/****** Declaraciones de datos externos **********************************************************/

/****** Declaración de funciones públicas ********************************************************/

boton_id_t	uBotonInicializar			( hal_pin_id_t );
bool			uBotonDesinicializar		( boton_id_t );
bool			uBotonActualizar 			( boton_id_t );
bool			uBotonActualizarTodo 	( void );

bool			uBotonPresionado 			( boton_id_t );
bool			uBotonNoPresionado 		( boton_id_t );
bool			uBotonPresionadoLargo	( boton_id_t );

bool			uBotonFlancoPresionando	( boton_id_t );
bool			uBotonFlancoSoltando 	( boton_id_t );

/*************************************************************************************************/
#endif /* __ISPEL_UBOTONES_H */
/****************************************************************** FIN DE ARCHIVO ***************/
