/**************************************************************************************************
 * Archivo: uOSAL.h
 * Breve:	Capa de abstracción de sistema operativo (OSAL).
 *          Incluye un mínimo de funciones de soporte.
 *          Creado para proyecto ISPEL.
 * Fecha:	Desarrollado en 2024
 * Autor:	Guillermo F. Caporaletti
 *
 *************************************************************************************************/

#ifndef ISPEL_UOSAL_H_
#define ISPEL_UOSAL_H_

/****** Librerías (includes) *********************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "uOSALconfig.h"

/****** Definiciones públicas (macros) ***********************************************************/


/****** Definiciones públicas de tipos de datos (public typedef) *********************************/

typedef bool bool_t;   // Esto es una finura :-)

/****** Declaraciones de datos externos **********************************************************/


/****** Declaración de funciones públicas ********************************************************/

bool     uoInicializar     	( void );

uint32_t	uoMilisegundos			( void );
uint32_t uoMicrosegundos   	( void );
void		uoEsperarMilis			( uint32_t );
void		uoEsperarMicros		( uint32_t );

void     uoLedEncender       	( uint16_t );
void     uoLedApagar         	( uint16_t );
void     uoLedInvertir       	( uint16_t );

void     uoEscribirTxt        ( const char * );
void     uoEscribirUint      	( uint32_t );
void     uoEscribirTxtTxt     ( const char *, const char * );
void     uoEscribirTxtUint		( const char *, uint32_t );
void     uoEscribirTxtUintTxt	( const char *, uint32_t, const char * );

void		uoHuboError 			( void );
void     uoHuboErrorTxt       ( const char * );

/*************************************************************************************************/
#endif /* ISPEL_UOSAL_H_ */
/****************************************************************** FIN DE ARCHIVO ***************/
