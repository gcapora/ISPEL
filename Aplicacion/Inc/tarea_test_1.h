/**************************************************************************************************
* @file		tarea_test_1.h
* @author	Guillermo Caporaletti
* @brief	Tarea para testeo
* @date		julio de 2023
*
**************************************************************************************************/

#ifndef __TAREA_TEST_1_H
#define __TAREA_TEST_1_H

#ifdef __cplusplus
 extern "C" {
#endif

/****** Librerías (includes) *********************************************************************/

#include "apli.h"		// Aquí están declarados los recursos compartidos
#include "tarea_leds.h"	// Tarea a testear

/****** Definiciones públicas (macros) ***********************************************************/


/****** Definiciones públicas de tipos de datos (public typedef) *********************************/


/****** Declaraciones de datos externos **********************************************************/

//extern led_id_t LedRojoEnPlaca;

/****** Declaración de funciones públicas ********************************************************/

bool TareaTestInicializar ( void );
void TareaTest_1 ( void * );
void TareaTest_2 ( void * );

/*************************************************************************************************/
#ifdef __cplusplus
}
#endif
#endif /* __TAREA_TEST_1_H */
/****************************************************************** FIN DE ARCHIVO ***************/
