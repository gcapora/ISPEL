/**************************************************************************************************
* @file		apli.h
* @author	Guillermo Caporaletti
* @brief
* @date
**************************************************************************************************/

#ifndef __ISPEL_APLI_H
#define __ISPEL_APLI_H

#ifdef __cplusplus
extern "C" {
#endif

/****** Librerías (includes) *********************************************************************/

#include "cmsis_os.h"	// ¿Por qué había que incluir esta librería?
#include "FreeRTOS.h"
#include "uOSAL.h"
#include "uCapturadora.h"
#include "uGenerador.h"
#include "Captu_RTOS.h"
#include "Leds_RTOS.h"
#include "Botones_RTOS.h"
#include "apli_interpretador.h"

/* Desarrollo includes. */
#include "tarea_test_1.h"
#include "supporting_Functions.h"

/****** Definiciones públicas (macros) ***********************************************************/

#define FREC_TESTIGO 		1000.0
#define UN_SEGUNDO			pdMS_TO_TICKS( 1000UL )
#define PERIODO_1MS			pdMS_TO_TICKS( 1UL )
#define PERIODO_10MS			pdMS_TO_TICKS( 10UL )
#define MENSAJE_LARGO_MAX	100

/****** Definiciones públicas de tipos de datos (public typedef) *********************************/


/****** Declaraciones de datos externos **********************************************************/

extern const char *			Barra;
extern boton_id_t  			BotonEnPlaca;
extern SemaphoreHandle_t	MutexApliEscribir;

/****** Declaración de funciones públicas ********************************************************/

void 			apli_inicializar( void );
void 			apli_alerta( const char * );
BaseType_t	apli_mensaje( const char *, TickType_t );
BaseType_t	tomar_escritura( TickType_t );
BaseType_t	devolver_escritura( void );

void Tarea_PALTA_1ms( void * );
void Tarea_PMEDIA_10ms( void * );

/*************************************************************************************************/
#ifdef __cplusplus
}
#endif
#endif/* __ISPEL_APLI_H */
/****************************************************************** FIN DE ARCHIVO ***************/
