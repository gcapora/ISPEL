/**************************************************************************************************
 * Archivo:   uHALdac.h
 * Breve:     Interfaz HAL para DACs
 * Autor:     Guillermo Caporaletti
 * Fecha:     creado en agosto 2023
 *************************************************************************************************/

#ifndef __ISPEL_UHALDAC_H
#define __ISPEL_UHALDAC_H

/****** Librerías (includes) *********************************************************************/

//#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/****** Definiciones públicas (macros) ***********************************************************/

#define UHAL_CANTIDAD_DACS  2

/****** Definiciones públicas de tipos de datos (public typedef) *********************************/

typedef enum {
	UHAL_DAC_1,
	UHAL_DAC_2
} dac_id_t;

typedef struct {
	uint32_t algo;  // para ver...
} dac_config_s;

/****** Declaraciones de datos externos **********************************************************/


/****** Declaración de funciones públicas ********************************************************/

void uHALdacdmaConfigurar ( dac_id_t, dac_config_s * );
void uHALdacdmaComenzar ( dac_id_t, uint32_t * , uint32_t );
void uHALdacdmaParar ( dac_id_t );

/*************************************************************************************************/
#endif /* __ISPEL_UHALDAC_H */
/****************************************************************** FIN DE ARCHIVO ***************/

