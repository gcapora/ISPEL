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

// Sistema operativo y capas intermedias
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "supporting_Functions.h"
#include "uOSAL.h"

// Módulos de aplicación
#include "Captu_RTOS.h"
#include "Gen_RTOS.h"
#include "Leds_RTOS.h"
#include "Botones_RTOS.h"
#include "Test_RTOS.h"
#include "apli_interpretador.h"

/****** Definiciones públicas (macros) ***********************************************************/

#define FREC_TESTIGO 			1000.0
#define FREC_MAX_SENIAL_GEN	100e3
#define UN_SEGUNDO				pdMS_TO_TICKS( 1000UL )
#define PERIODO_1MS				pdMS_TO_TICKS( 1UL )
#define PERIODO_10MS				pdMS_TO_TICKS( 10UL )
#define TIEMPO_SEPARADOR		1000 // en milisegundos
#define MENSAJE_LARGO_MAX		100

/****** Definiciones públicas de tipos de datos (public typedef) *********************************/


/****** Declaración de datos publicos (public data) **********************************************/

extern bool_t					EquipoEncendido;
extern const char *			Barra;
extern boton_id_t  			BotonEnPlaca, BotonCaptuDisparo;
extern SemaphoreHandle_t	MutexApliEscribir;

/****** Declaración de funciones públicas (public functions) **************************************/

void 			apli_inicializar( void );
void 			apli_alerta( const char * );
BaseType_t	apli_mensaje( const char *, TickType_t );
void			apli_separador(const char * );
void			apli_latido(void);
BaseType_t	tomar_escritura( TickType_t );
BaseType_t	devolver_escritura( void );

void Tarea_PALTA( void * );
void Tarea_PMEDIA( void * );

/*************************************************************************************************/
#ifdef __cplusplus
}
#endif
#endif/* __ISPEL_APLI_H */
/****************************************************************** FIN DE ARCHIVO ***************/
