/**************************************************************************************************
* @file		apli.h
* @author	Guillermo Caporaletti
* @brief
* @date
**************************************************************************************************/

#ifndef __ISPEL_APLI_INTERPRETADOR_H
#define __ISPEL_APLI_INTERPRETADOR_H

#ifdef __cplusplus
extern "C" {
#endif

/****** Librerías (includes) *********************************************************************/

#include <stdbool.h>
#include "apli.h"

/****** Definiciones públicas (macros) ***********************************************************/

#define CMD_HOLA				"HOLA"
#define CMD_CAPTURADORA		"CAPTU"
#define CMD_ENTRADA			"ENTRADA"
#define CMD_GENERADOR		"GEN"
#define CMD_CONFIGURAR		"CONFIG"
#define CMD_OBTENER			"OBTENER"
#define CMD_INICIAR			"INICIAR"
#define CMD_PARAR				"PARAR"
#define CMD_ENCENDER			"ENCENDER"
#define CMD_APAGAR			"APAGAR"
#define CMD_ESCRIBIR			"ESCRIBIR"
#define CMD_LEER			   "LEER"

/****** Definiciones públicas de tipos de datos (public typedef) *********************************/


/****** Declaraciones de datos externos **********************************************************/


/****** Declaración de funciones públicas ********************************************************/

bool			ai_inicializar( void );
void			ai_cargar_caracter( char );
void			ai_cargar_mensaje( char *, uint32_t );
bool			ai_procesar_mensajes( void );

/*************************************************************************************************/
#ifdef __cplusplus
}
#endif
#endif/* __ISPEL_APLI_H */
/****************************************************************** FIN DE ARCHIVO ***************/
