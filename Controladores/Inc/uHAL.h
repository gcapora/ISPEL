/**************************************************************************************************
 * Archivo: uHAL.h
 * Breve:	Interfaz de abstracción de hardware (HAL). Proyecto ISPEL.
 * Fecha:	Creado en julio 2023
 * Autor:	Guillermo F. Caporaletti
 *
 *************************************************************************************************/

#ifndef ISPEL_UHAL_H_
#define ISPEL_UHAL_H_

/****** Librerías (includes) *********************************************************************/

#include "uHALgpio.h"
#include "uHALdac.h"

/****** Definiciones públicas (macros) ***********************************************************/

// Nombres específicos de pines -------------------------------------------------------------------
#define U_LED_VERDE_EP HAL_PIN_PB0
#define U_LED_AZUL_EP  HAL_PIN_PB7
#define U_LED_ROJO_EP  HAL_PIN_PB14
#define U_BOTON_EP     HAL_PIN_PC13

/****** Definiciones públicas de tipos de datos (public typedef) *********************************/


/****** Declaraciones de datos externos **********************************************************/


/****** Declaración de funciones públicas ********************************************************/


/*************************************************************************************************/
#endif /* ISPEL_UHAL_H_ */
/****************************************************************** FIN DE ARCHIVO ***************/
