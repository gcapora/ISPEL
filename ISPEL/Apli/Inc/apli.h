/* Interfaz electrónica para prácticas de laboratorio
 *
 * Año:     2023
 * Autor:   Guillermo F. Caporaletti
 * Archivo: apli.h
 *
 */

#ifndef __MAIN_APP_H
#define __MAIN_APP_H

#ifdef __cplusplus
extern "C" {
#endif

// ------ inclusions ---------------------------------------------------
#include "cmsis_os.h"	// ¿Por qué había que incluir esta librería?
#include "FreeRTOS.h"
#include "tarea_leds.h"

/* Desarrollo includes. */
#include "tarea_test_1.h"
#include "supporting_Functions.h"

// ------ macros -------------------------------------------------------
// ------ typedef ------------------------------------------------------
// ------ external data declaration ------------------------------------
// TaskHandle_t xTaskLedHandle;

// ------ external functions declaration -------------------------------
void apliInicializar( void );

#ifdef __cplusplus
}
#endif

#endif