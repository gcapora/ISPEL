/* Interfaz electrónica para prácticas de laboratorio
 *
 * Año:     2023
 * Autor:   Guillermo F. Caporaletti
 * Archivo: main_app.h
 *
 */

#ifndef __MAIN_APP_H
#define __MAIN_APP_H

#ifdef __cplusplus
extern "C" {
#endif

// ------ inclusions ---------------------------------------------------
#include "cmsis_os.h"


// ------ macros -------------------------------------------------------
// ------ typedef ------------------------------------------------------
// ------ external data declaration ------------------------------------
// TaskHandle_t xTaskLedHandle;

// ------ external functions declaration -------------------------------
 void appInit( void );

#ifdef __cplusplus
}
#endif

#endif
