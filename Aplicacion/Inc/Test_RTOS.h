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

#include "apli.h"

/****** Definiciones públicas (macros) ***********************************************************/


/****** Definiciones públicas de tipos de datos (public typedef) *********************************/

 typedef enum {
 	TEST_GENERAL,
 	NUM_TEST,
 } tipo_test_e;

/****** Declaraciones de datos externos **********************************************************/


/****** Declaración de funciones públicas ********************************************************/

bool TestRTOS_Inicializar	( void );
void Tarea_Test 				( void * );
void Test_Testear				( tipo_test_e, TickType_t );
void Test_Config_0			( TickType_t );
void Test_Captu_Inicia		( TickType_t );
void Test_Gen_Enciende		( TickType_t );

/*************************************************************************************************/
#ifdef __cplusplus
}
#endif
#endif /* __TAREA_TEST_1_H */
/****************************************************************** FIN DE ARCHIVO ***************/
