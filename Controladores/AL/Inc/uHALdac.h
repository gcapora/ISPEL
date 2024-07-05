/**************************************************************************************************
 * Archivo:   uHALdac.h
 * Breve:     Interfaz HAL para DACs
 * Autor:     Guillermo Caporaletti
 * Fecha:     creado en agosto 2023
 *
 * Descripción:
 *    La librería brinda las funciones necesarias para el manejo de hardware de dos DACs.
 *    Está implementada inicialmente sobre STM32 Nucleo-429ZI.
 *    La librería inlcuye algunas definiciones específicas de esta placa.
 *    En una posterior versión, se debe trasladar las definiciones específicas a uHALdac_conf.h.
 *
 * Problemas:
 *  - La función uHALdacdmaSincronizar() puede sacar de servicio un tempo si ambas señales no
 *    tienen la misma frecuencia de muestreo. Se incluye una verificación antes de actuar.
 *
 * Mejoras:
 *  - Se debe incorporar la posibilidad de configurar el alineamiento de datos dentro del vector de
 *    32 bits donde está almacenada la señal.
 *  - Para sincronizar, se apagan las señales, se paran los tempos y luego se inicia todos junto.
 *    Se puede sincronizar "al vuelo" si se implementa un sincronismo con un temporizador extra.
 *
 *************************************************************************************************/

#ifndef __ISPEL_UHALDAC_H
#define __ISPEL_UHALDAC_H

/****** Librerías (includes) *********************************************************************/

#include "uHAL.h"

/****** Definiciones públicas (macros) ***********************************************************/

#define UHAL_CANTIDAD_DACS          		2
#define UHAL_FRECUENCIA_RELOJ					UOSAL_FRECUENCIA_RELOJ // Hz
#define UHAL_DAC_FRECUENCIA_MUESTREO_MAX	4000000   // Considera un margen.
#define MILLON                      		1000000   // 1 millon :-)
#define MHZ                        		 	MILLON    // 1 megahertz
#define KHZ                         		1000      // 1 kilohertz

/****** Definiciones públicas de tipos de datos (public typedef) *********************************/

typedef enum {
	UHAL_DAC_1,
	UHAL_DAC_2,
	UHAL_DAC_TODOS
} dac_id_e;

/****** Declaración de datos públicos ************************************************************/

extern const uint32_t MAXIMO_DAC[UHAL_CANTIDAD_DACS]; // Valores máximos recomendables para DAC's (<=4095)
extern const uint32_t MINIMO_DAC[UHAL_CANTIDAD_DACS]; // Valores mínimos recomendables para DAC's (>=0)
extern const uint32_t CERO_DAC[UHAL_CANTIDAD_DACS];
extern const double   TRANSFERENCIA_DAC[UHAL_CANTIDAD_DACS]; // Voltios / cuenta (aprox. 805,9 uV/cuenta)

/****** Declaración de funciones públicas ********************************************************/

bool     uHALdacInicializar ( dac_id_e );
bool     uHALdacEstablecerValor ( dac_id_e, uint32_t);
bool     uHALdacParar ( dac_id_e );
bool     uHALdacdmaComenzar ( dac_id_e, uint32_t *, uint32_t );
double   uHALdacdmaConfigurarFrecuenciaMuestreo ( dac_id_e, double );
double   uHALdacdmaLeerFrecuenciaMuestreo ( dac_id_e );
double   uHALdacdmaLeerFrecuenciaBase (void);
double   uHALdacdmaLeerFrecuenciaMaxima (void);
double   uHALdacdmaLeerFrecuenciaMinima (void);
uint32_t uHALdacdmaLeerDivisor (dac_id_e);
bool     uHALdacdmaSincronizar ( void );

/*************************************************************************************************/
#endif /* __ISPEL_UHALDAC_H */
/****************************************************************** FIN DE ARCHIVO ***************/

