/* Interfaz electrónica para prácticas de laboratorio
 *
 * Año:     2023
 * Autor:   Guillermo F. Caporaletti
 * Archivo: apli.h
 *
 */

#ifndef __ISPEL_APLI_H
#define __ISPEL_APLI_H

#ifdef __cplusplus
extern "C" {
#endif

// ------ inclusions ---------------------------------------------------

#include "cmsis_os.h"	// ¿Por qué había que incluir esta librería?
#include "FreeRTOS.h"
#include "uOSAL.h"
#include "uCapturadora.h"
#include "tarea_leds.h"
#include "tarea_botones.h"

/* Desarrollo includes. */
#include "tarea_test_1.h"
#include "supporting_Functions.h"

// ------ macros -------------------------------------------------------

#define UN_SEGUNDO			pdMS_TO_TICKS( 1000UL )
#define FREC_TESTIGO 		10000.0

// ------ typedef ------------------------------------------------------


// ------ external data declaration ------------------------------------

extern const char *Barra;

// TaskHandle_t xTaskLedHandle;

// ------ public functions declaration -------------------------------

void apliInicializar( void );
void ImprimirSenial32_main (void);

#ifdef __cplusplus
}
#endif

#endif
