/**************************************************************************************************
* @file		apli_interpretador.h
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

#include "apli.h"

/****** Definiciones públicas (macros) ***********************************************************/

#define CMD_HOLA				"HOLA"
#define CMD_CAPTURADORA		"CAPTU"
#define CMD_ENTRADA_1		"E1"
#define CMD_ENTRADA_2		"E2"
#define CMD_ENTRADA_X		"EX"
#define CMD_GENERADOR		"GEN"
#define CMD_SALIDA_1			"S1"
#define CMD_SALIDA_2			"S2"
#define CMD_SALIDA_X			"SX"
#define CMD_CONFIGURAR		"CONFIG"
#define CMD_OBTENER			"OBTENER"
#define CMD_INICIAR			"INICIAR"
#define CMD_PARAR				"PARAR"
#define CMD_ENCENDER			"ENCENDER"
#define CMD_APAGAR			"APAGAR"
#define CMD_ESCRIBIR			"ESCRIBIR"
#define CMD_LEER			   "LEER"
#define CMD_MENSAJE			"MSJ"
#define CMD_MENSAJE2			"//"
#define CMD_ESPERAR			"ESPERAR"
#define CMD_TEST				"TEST"

#define MENSAJE_LARGO_MAX	100
#define VALOR_LARGO_MAX		16

/****** Definiciones públicas de tipos de datos (public typedef) *********************************/

typedef enum {
	TIPO,
	FREC_SENIAL,
	FASE_SENIAL,
	VMAX_SENIAL,
	VMIN_SENIAL,
	SIM_SENIAL,
	ACOPLE_SENIAL,
	ESCALA,
	ORIGEN,
	PROMEDIO,
	NIVEL,
	FLANCO,
	NUM_ATRIBUTOS,
	ATRIBUTO_NO_IDENTIFICADO
} atributo_e;

/****** Declaraciones de datos externos **********************************************************/

extern char * ATRIBUTO_ID_TXT[];

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
