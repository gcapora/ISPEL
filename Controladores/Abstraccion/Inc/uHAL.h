/**************************************************************************************************
 * Archivo: uHAL.h
 * Breve:	Encabeado general de funciones uHAL.
 * Fecha:
 * Autor:	Guillermo F. Caporaletti
 *
 *************************************************************************************************/

#ifndef ISPEL_UHAL_H_
#define ISPEL_UHAL_H_

/****** Librerías (includes) *********************************************************************/

#include <stdio.h>
#include <string.h>	// No es imprescindible pero preferible
#include "uOSAL.h"
#include "uHALconfig.h"
#include "uHALgpio.h"
#include "uHALdac.h"
#include "uHALmap.h"
#include "uHALadc.h"

/****** Definiciones públicas (macros) ***********************************************************/


/****** Definiciones públicas de tipos de datos (public typedef) *********************************/


/****** Declaraciones de datos externos **********************************************************/


/****** Declaración de funciones públicas ********************************************************/

bool	uHALinicializar       ( void );

/*************************************************************************************************/
#endif /* ISPEL_UHAL_H_ */
/****************************************************************** FIN DE ARCHIVO ***************/
