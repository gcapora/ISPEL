/**************************************************************************************************
 * Archivo: uHALgpio.h
 * Breve:	Interfaz de abstracción de hardware (HAL) para
 *          pines de entrada-salida de propósito general (GPIO).
 *          Proyecto ISPEL.
 * Fecha:	2024
 * Autor:	Guillermo F. Caporaletti
 *
 *************************************************************************************************/

#ifndef ISPEL_UHALGPIO_H_
#define ISPEL_UHALGPIO_H_

/****** Librerías (includes) *********************************************************************/

#include "uHAL.h"

/****** Definiciones públicas (macros) ***********************************************************/

// Configuraciones de GPIO
#define U_GPIO_MODO_ENTRADA		0x0UL
#define U_GPIO_MODO_SALIDA			0x1UL
#define U_GPIO_MODO_SALIDA_TE		U_GPIO_MODO_SALIDA | (0x0UL << 4U) // Tirar-empujar (push-pull)
#define U_GPIO_MODO_SALIDA_DA		U_GPIO_MODO_SALIDA | (0x1UL << 4U) // Drenador-abierto (open-drain)
#define U_GPIO_NO_TIRAR	    		0x00000000U
#define U_GPIO_TIRAR_ARRIBA		0x00000001U
#define U_GPIO_TIRAR_ABAJO			0x00000002U
#define U_GPIO_VELOCIDAD_BAJA		0x00000000U
#define U_GPIO_VELOCIDAD_MEDIA	0x00000001U
#define U_GPIO_VELOCIDAD_ALTA		0x00000002U

/****** Definiciones públicas de tipos de datos (public typedef) *********************************/

// Puntero que IDENTIFICA el pin ------------------------------------------------------------------
typedef struct hal_pin_id_s const * hal_pin_id_t;

// Estructura de CONFIGURACION de GPIO ------------------------------------------------------------
// (similar a GPIO_InitTypeDef en stm32f4xx_hal_gpio.h)
typedef struct
{
  uint32_t Modo;      // Specifies the operating mode for the selected pins.
  uint32_t Tirar;     // Specifies the Pull-up or Pull-Down activation for the selected pins.
  uint32_t Velocidad; // Specifies the speed for the selected pins.
  //uint32_t Alternar;  // Peripheral to be connected to the selected pins.
} hal_pin_config_s;

/****** Declaraciones de datos externos **********************************************************/

extern const hal_pin_id_t HAL_PIN_PA0;  /**< Constant to define Pin 0 on chip port A */
extern const hal_pin_id_t HAL_PIN_PA1;  /**< Constant to define Pin 1 on chip port A */
extern const hal_pin_id_t HAL_PIN_PA2;  /**< Constant to define Pin 2 on chip port A */
extern const hal_pin_id_t HAL_PIN_PA3;  /**< Constant to define Pin 3 on chip port A */
extern const hal_pin_id_t HAL_PIN_PA4;  /**< Constant to define Pin 4 on chip port A */
extern const hal_pin_id_t HAL_PIN_PA5;  /**< Constant to define Pin 5 on chip port A */
extern const hal_pin_id_t HAL_PIN_PA6;  /**< Constant to define Pin 6 on chip port A */
extern const hal_pin_id_t HAL_PIN_PA7;  /**< Constant to define Pin 7 on chip port A */
extern const hal_pin_id_t HAL_PIN_PA8;  /**< Constant to define Pin 8 on chip port A */
extern const hal_pin_id_t HAL_PIN_PA9;  /**< Constant to define Pin 9 on chip port A */
extern const hal_pin_id_t HAL_PIN_PA10; /**< Constant to define Pin 10 on chip port A */
extern const hal_pin_id_t HAL_PIN_PA11; /**< Constant to define Pin 11 on chip port A */
extern const hal_pin_id_t HAL_PIN_PA12; /**< Constant to define Pin 12 on chip port A */
extern const hal_pin_id_t HAL_PIN_PA13; /**< Constant to define Pin 13 on chip port A */
extern const hal_pin_id_t HAL_PIN_PA14; /**< Constant to define Pin 14 on chip port A */
extern const hal_pin_id_t HAL_PIN_PA15; /**< Constant to define Pin 15 on chip port A */

extern const hal_pin_id_t HAL_PIN_PB0;  /**< Constant to define Pin 0 on chip port B */
extern const hal_pin_id_t HAL_PIN_PB1;  /**< Constant to define Pin 1 on chip port B */
extern const hal_pin_id_t HAL_PIN_PB2;  /**< Constant to define Pin 2 on chip port B */
extern const hal_pin_id_t HAL_PIN_PB3;  /**< Constant to define Pin 3 on chip port B */
extern const hal_pin_id_t HAL_PIN_PB4;  /**< Constant to define Pin 4 on chip port B */
extern const hal_pin_id_t HAL_PIN_PB5;  /**< Constant to define Pin 5 on chip port B */
extern const hal_pin_id_t HAL_PIN_PB6;  /**< Constant to define Pin 6 on chip port B */
extern const hal_pin_id_t HAL_PIN_PB7;  /**< Constant to define Pin 7 on chip port B */
extern const hal_pin_id_t HAL_PIN_PB8;  /**< Constant to define Pin 8 on chip port B */
extern const hal_pin_id_t HAL_PIN_PB9;  /**< Constant to define Pin 9 on chip port B */
extern const hal_pin_id_t HAL_PIN_PB10; /**< Constant to define Pin 10 on chip port B */
extern const hal_pin_id_t HAL_PIN_PB11; /**< Constant to define Pin 11 on chip port B */
extern const hal_pin_id_t HAL_PIN_PB12; /**< Constant to define Pin 12 on chip port B */
extern const hal_pin_id_t HAL_PIN_PB13; /**< Constant to define Pin 13 on chip port B */
extern const hal_pin_id_t HAL_PIN_PB14; /**< Constant to define Pin 14 on chip port B */
extern const hal_pin_id_t HAL_PIN_PB15; /**< Constant to define Pin 15 on chip port B */

extern const hal_pin_id_t HAL_PIN_PC0;
extern const hal_pin_id_t HAL_PIN_PC1;
extern const hal_pin_id_t HAL_PIN_PC2;
extern const hal_pin_id_t HAL_PIN_PC3;
extern const hal_pin_id_t HAL_PIN_PC4;
extern const hal_pin_id_t HAL_PIN_PC5;
extern const hal_pin_id_t HAL_PIN_PC6;
extern const hal_pin_id_t HAL_PIN_PC7;
extern const hal_pin_id_t HAL_PIN_PC8;
extern const hal_pin_id_t HAL_PIN_PC9;
extern const hal_pin_id_t HAL_PIN_PC10;
extern const hal_pin_id_t HAL_PIN_PC11;
extern const hal_pin_id_t HAL_PIN_PC12;
extern const hal_pin_id_t HAL_PIN_PC13;
extern const hal_pin_id_t HAL_PIN_PC14;
extern const hal_pin_id_t HAL_PIN_PC15;

extern const hal_pin_id_t HAL_PIN_PD0;
extern const hal_pin_id_t HAL_PIN_PD1;
extern const hal_pin_id_t HAL_PIN_PD2;
extern const hal_pin_id_t HAL_PIN_PD3;
extern const hal_pin_id_t HAL_PIN_PD4;
extern const hal_pin_id_t HAL_PIN_PD5;
extern const hal_pin_id_t HAL_PIN_PD6;
extern const hal_pin_id_t HAL_PIN_PD7;
extern const hal_pin_id_t HAL_PIN_PD8;
extern const hal_pin_id_t HAL_PIN_PD9;
extern const hal_pin_id_t HAL_PIN_PD10;
extern const hal_pin_id_t HAL_PIN_PD11;
extern const hal_pin_id_t HAL_PIN_PD12;
extern const hal_pin_id_t HAL_PIN_PD13;
extern const hal_pin_id_t HAL_PIN_PD14;
extern const hal_pin_id_t HAL_PIN_PD15;

extern const hal_pin_id_t HAL_PIN_PE0;
extern const hal_pin_id_t HAL_PIN_PE1;
extern const hal_pin_id_t HAL_PIN_PE2;
extern const hal_pin_id_t HAL_PIN_PE3;
extern const hal_pin_id_t HAL_PIN_PE4;
extern const hal_pin_id_t HAL_PIN_PE5;
extern const hal_pin_id_t HAL_PIN_PE6;
extern const hal_pin_id_t HAL_PIN_PE7;
extern const hal_pin_id_t HAL_PIN_PE8;
extern const hal_pin_id_t HAL_PIN_PE9;
extern const hal_pin_id_t HAL_PIN_PE10;
extern const hal_pin_id_t HAL_PIN_PE11;
extern const hal_pin_id_t HAL_PIN_PE12;
extern const hal_pin_id_t HAL_PIN_PE13;
extern const hal_pin_id_t HAL_PIN_PE14;
extern const hal_pin_id_t HAL_PIN_PE15;

extern const hal_pin_id_t HAL_PIN_PF0;
extern const hal_pin_id_t HAL_PIN_PF1;
extern const hal_pin_id_t HAL_PIN_PF2;
extern const hal_pin_id_t HAL_PIN_PF3;
extern const hal_pin_id_t HAL_PIN_PF4;
extern const hal_pin_id_t HAL_PIN_PF5;
extern const hal_pin_id_t HAL_PIN_PF6;
extern const hal_pin_id_t HAL_PIN_PF7;
extern const hal_pin_id_t HAL_PIN_PF8;
extern const hal_pin_id_t HAL_PIN_PF9;
extern const hal_pin_id_t HAL_PIN_PF10;
extern const hal_pin_id_t HAL_PIN_PF11;
extern const hal_pin_id_t HAL_PIN_PF12;
extern const hal_pin_id_t HAL_PIN_PF13;
extern const hal_pin_id_t HAL_PIN_PF14;
extern const hal_pin_id_t HAL_PIN_PF15;

extern const hal_pin_id_t HAL_PIN_PG0;
extern const hal_pin_id_t HAL_PIN_PG1;
extern const hal_pin_id_t HAL_PIN_PG2;
extern const hal_pin_id_t HAL_PIN_PG3;
extern const hal_pin_id_t HAL_PIN_PG4;
extern const hal_pin_id_t HAL_PIN_PG5;
extern const hal_pin_id_t HAL_PIN_PG6;
extern const hal_pin_id_t HAL_PIN_PG7;
extern const hal_pin_id_t HAL_PIN_PG8;
extern const hal_pin_id_t HAL_PIN_PG9;
extern const hal_pin_id_t HAL_PIN_PG10;
extern const hal_pin_id_t HAL_PIN_PG11;
extern const hal_pin_id_t HAL_PIN_PG12;
extern const hal_pin_id_t HAL_PIN_PG13;
extern const hal_pin_id_t HAL_PIN_PG14;
extern const hal_pin_id_t HAL_PIN_PG15;

extern const hal_pin_id_t HAL_PIN_PH0;
extern const hal_pin_id_t HAL_PIN_PH1;
extern const hal_pin_id_t HAL_PIN_PH2;
extern const hal_pin_id_t HAL_PIN_PH3;
extern const hal_pin_id_t HAL_PIN_PH4;
extern const hal_pin_id_t HAL_PIN_PH5;
extern const hal_pin_id_t HAL_PIN_PH6;
extern const hal_pin_id_t HAL_PIN_PH7;
extern const hal_pin_id_t HAL_PIN_PH8;
extern const hal_pin_id_t HAL_PIN_PH9;
extern const hal_pin_id_t HAL_PIN_PH10;
extern const hal_pin_id_t HAL_PIN_PH11;
extern const hal_pin_id_t HAL_PIN_PH12;
extern const hal_pin_id_t HAL_PIN_PH13;
extern const hal_pin_id_t HAL_PIN_PH14;
extern const hal_pin_id_t HAL_PIN_PH15;

extern const hal_pin_id_t HAL_PIN_PI0;
extern const hal_pin_id_t HAL_PIN_PI1;
extern const hal_pin_id_t HAL_PIN_PI2;
extern const hal_pin_id_t HAL_PIN_PI3;
extern const hal_pin_id_t HAL_PIN_PI4;
extern const hal_pin_id_t HAL_PIN_PI5;
extern const hal_pin_id_t HAL_PIN_PI6;
extern const hal_pin_id_t HAL_PIN_PI7;
extern const hal_pin_id_t HAL_PIN_PI8;
extern const hal_pin_id_t HAL_PIN_PI9;
extern const hal_pin_id_t HAL_PIN_PI10;
extern const hal_pin_id_t HAL_PIN_PI11;
extern const hal_pin_id_t HAL_PIN_PI12;
extern const hal_pin_id_t HAL_PIN_PI13;
extern const hal_pin_id_t HAL_PIN_PI14;
extern const hal_pin_id_t HAL_PIN_PI15;

extern const hal_pin_id_t HAL_PIN_PJ0;
extern const hal_pin_id_t HAL_PIN_PJ1;
extern const hal_pin_id_t HAL_PIN_PJ2;
extern const hal_pin_id_t HAL_PIN_PJ3;
extern const hal_pin_id_t HAL_PIN_PJ4;
extern const hal_pin_id_t HAL_PIN_PJ5;
extern const hal_pin_id_t HAL_PIN_PJ6;
extern const hal_pin_id_t HAL_PIN_PJ7;
extern const hal_pin_id_t HAL_PIN_PJ8;
extern const hal_pin_id_t HAL_PIN_PJ9;
extern const hal_pin_id_t HAL_PIN_PJ10;
extern const hal_pin_id_t HAL_PIN_PJ11;
extern const hal_pin_id_t HAL_PIN_PJ12;
extern const hal_pin_id_t HAL_PIN_PJ13;
extern const hal_pin_id_t HAL_PIN_PJ14;
extern const hal_pin_id_t HAL_PIN_PJ15;

extern const hal_pin_id_t HAL_PIN_PK0;
extern const hal_pin_id_t HAL_PIN_PK1;
extern const hal_pin_id_t HAL_PIN_PK2;
extern const hal_pin_id_t HAL_PIN_PK3;
extern const hal_pin_id_t HAL_PIN_PK4;
extern const hal_pin_id_t HAL_PIN_PK5;
extern const hal_pin_id_t HAL_PIN_PK6;
extern const hal_pin_id_t HAL_PIN_PK7;
extern const hal_pin_id_t HAL_PIN_PK8;
extern const hal_pin_id_t HAL_PIN_PK9;
extern const hal_pin_id_t HAL_PIN_PK10;
extern const hal_pin_id_t HAL_PIN_PK11;
extern const hal_pin_id_t HAL_PIN_PK12;
extern const hal_pin_id_t HAL_PIN_PK13;
extern const hal_pin_id_t HAL_PIN_PK14;
extern const hal_pin_id_t HAL_PIN_PK15;

/****** Declaración de funciones públicas ********************************************************/

void uHALgpioInicializar ( hal_pin_id_t , hal_pin_config_s * );
bool uHALgpioLeer        ( hal_pin_id_t );
void uHALgpioEscribir    ( hal_pin_id_t , bool );
void uHALgpioInvertir    ( hal_pin_id_t );

/*************************************************************************************************/
#endif /* ISPEL_UHALGPIO_H_ */
/****************************************************************** FIN DE ARCHIVO ***************/
