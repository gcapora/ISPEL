/**************************************************************************************************
 * Archivo: uHAL.c
 * Breve:	Código de interfaz de abstracción de hardware (HAL). Proyecto ISPEL.
 * Fecha:	Creado julio 2023
 * Autor:	Guillermo F. Caporaletti
 *
 *************************************************************************************************/

/****** Librerías (includes) *********************************************************************/

#include "uHAL.h"
#include "stm32f4xx_hal.h"

/****** Definiciones privadas (macros) ***********************************************************/

// Constantes numéricas asignadas a puertos E/S
#define HAL_PORT_A 0
#define HAL_PORT_B 1
#define HAL_PORT_C 2
#define HAL_PORT_D 3
#define HAL_PORT_E 4
#define HAL_PORT_F 5
#define HAL_PORT_G 6
#define HAL_PORT_H 7
#define HAL_PORT_I 8
#define HAL_PORT_J 9
#define HAL_PORT_K 10

// Macros para generar nombre de PIN, PUERTO y CHIP_PIN:
#define PIN_NAME(PORT, PIN) HAL_PIN_P##PORT##PIN
#define PORT_NAME(PORT) HAL_PORT_##PORT
#define CHIP_PIN(PORT, PIN)	PIN_NAME(PORT, PIN) = &(struct hal_pin_id_s) { .port = PORT_NAME(PORT), .pin = PIN }

/****** Definiciones privadas de tipos (private typedef) *****************************************/

// Estructura de pin de chip STM32 y su referencia por puntero
struct hal_pin_id_s {
    uint8_t pin  : 4; /**< Number of chip pin port */
    uint8_t port : 4; /**< Number of pin in chip port */
};

/****** Definición de variables y constantes públicas ********************************************/

// Constantes para referenciar a pines en pin y puerto del chip específico ------------------------
const hal_pin_id_t CHIP_PIN(A, 0);  /**< Constant to define Pin 0 on chip port A */
// Esto equivale a:
// const hal_pin_id_t HAL_PIN_PA0 = &(struct hal_pin_id_s) { .port = HAL_PORT_A, .pin = 0 };
// Crea una estructura y un puntero constante a esa estructura.

const hal_pin_id_t CHIP_PIN(A, 1);  /**< Constant to define Pin 1 on chip port A */
const hal_pin_id_t CHIP_PIN(A, 2);  /**< Constant to define Pin 2 on chip port A */
const hal_pin_id_t CHIP_PIN(A, 3);  /**< Constant to define Pin 3 on chip port A */
const hal_pin_id_t CHIP_PIN(A, 4);  /**< Constant to define Pin 4 on chip port A */
const hal_pin_id_t CHIP_PIN(A, 5);  /**< Constant to define Pin 6 on chip port A */
const hal_pin_id_t CHIP_PIN(A, 6);  /**< Constant to define Pin 7 on chip port A */
const hal_pin_id_t CHIP_PIN(A, 7);  /**< Constant to define Pin 8 on chip port A */
const hal_pin_id_t CHIP_PIN(A, 8);  /**< Constant to define Pin 9 on chip port A */
const hal_pin_id_t CHIP_PIN(A, 9);  /**< Constant to define Pin 10 on chip port A */
const hal_pin_id_t CHIP_PIN(A, 10); /**< Constant to define Pin 11 on chip port A */
const hal_pin_id_t CHIP_PIN(A, 11); /**< Constant to define Pin 12 on chip port A */
const hal_pin_id_t CHIP_PIN(A, 12); /**< Constant to define Pin 13 on chip port A */
const hal_pin_id_t CHIP_PIN(A, 13); /**< Constant to define Pin 14 on chip port A */
const hal_pin_id_t CHIP_PIN(A, 14); /**< Constant to define Pin 15 on chip port A */
const hal_pin_id_t CHIP_PIN(A, 15); /**< Constant to define Pin 16 on chip port A */

const hal_pin_id_t CHIP_PIN(B, 0);
const hal_pin_id_t CHIP_PIN(B, 1);
const hal_pin_id_t CHIP_PIN(B, 2);
const hal_pin_id_t CHIP_PIN(B, 3);
const hal_pin_id_t CHIP_PIN(B, 4);
const hal_pin_id_t CHIP_PIN(B, 5);
const hal_pin_id_t CHIP_PIN(B, 6);
const hal_pin_id_t CHIP_PIN(B, 7);
const hal_pin_id_t CHIP_PIN(B, 8);
const hal_pin_id_t CHIP_PIN(B, 9);
const hal_pin_id_t CHIP_PIN(B, 10);
const hal_pin_id_t CHIP_PIN(B, 11);
const hal_pin_id_t CHIP_PIN(B, 12);
const hal_pin_id_t CHIP_PIN(B, 13);
const hal_pin_id_t CHIP_PIN(B, 14);
const hal_pin_id_t CHIP_PIN(B, 15);

const hal_pin_id_t CHIP_PIN(C, 0);
const hal_pin_id_t CHIP_PIN(C, 1);
const hal_pin_id_t CHIP_PIN(C, 2);
const hal_pin_id_t CHIP_PIN(C, 3);
const hal_pin_id_t CHIP_PIN(C, 4);
const hal_pin_id_t CHIP_PIN(C, 5);
const hal_pin_id_t CHIP_PIN(C, 6);
const hal_pin_id_t CHIP_PIN(C, 7);
const hal_pin_id_t CHIP_PIN(C, 8);
const hal_pin_id_t CHIP_PIN(C, 9);
const hal_pin_id_t CHIP_PIN(C, 10);
const hal_pin_id_t CHIP_PIN(C, 11);
const hal_pin_id_t CHIP_PIN(C, 12);
const hal_pin_id_t CHIP_PIN(C, 13);
const hal_pin_id_t CHIP_PIN(C, 14);
const hal_pin_id_t CHIP_PIN(C, 15);

const hal_pin_id_t CHIP_PIN(D, 0);
const hal_pin_id_t CHIP_PIN(D, 1);
const hal_pin_id_t CHIP_PIN(D, 2);
const hal_pin_id_t CHIP_PIN(D, 3);
const hal_pin_id_t CHIP_PIN(D, 4);
const hal_pin_id_t CHIP_PIN(D, 5);
const hal_pin_id_t CHIP_PIN(D, 6);
const hal_pin_id_t CHIP_PIN(D, 7);
const hal_pin_id_t CHIP_PIN(D, 8);
const hal_pin_id_t CHIP_PIN(D, 9);
const hal_pin_id_t CHIP_PIN(D, 10);
const hal_pin_id_t CHIP_PIN(D, 11);
const hal_pin_id_t CHIP_PIN(D, 12);
const hal_pin_id_t CHIP_PIN(D, 13);
const hal_pin_id_t CHIP_PIN(D, 14);
const hal_pin_id_t CHIP_PIN(D, 15);

const hal_pin_id_t CHIP_PIN(E, 0);
const hal_pin_id_t CHIP_PIN(E, 1);
const hal_pin_id_t CHIP_PIN(E, 2);
const hal_pin_id_t CHIP_PIN(E, 3);
const hal_pin_id_t CHIP_PIN(E, 4);
const hal_pin_id_t CHIP_PIN(E, 5);
const hal_pin_id_t CHIP_PIN(E, 6);
const hal_pin_id_t CHIP_PIN(E, 7);
const hal_pin_id_t CHIP_PIN(E, 8);
const hal_pin_id_t CHIP_PIN(E, 9);
const hal_pin_id_t CHIP_PIN(E, 10);
const hal_pin_id_t CHIP_PIN(E, 11);
const hal_pin_id_t CHIP_PIN(E, 12);
const hal_pin_id_t CHIP_PIN(E, 13);
const hal_pin_id_t CHIP_PIN(E, 14);
const hal_pin_id_t CHIP_PIN(E, 15);

const hal_pin_id_t CHIP_PIN(F, 0);
const hal_pin_id_t CHIP_PIN(F, 1);
const hal_pin_id_t CHIP_PIN(F, 2);
const hal_pin_id_t CHIP_PIN(F, 3);
const hal_pin_id_t CHIP_PIN(F, 4);
const hal_pin_id_t CHIP_PIN(F, 5);
const hal_pin_id_t CHIP_PIN(F, 6);
const hal_pin_id_t CHIP_PIN(F, 7);
const hal_pin_id_t CHIP_PIN(F, 8);
const hal_pin_id_t CHIP_PIN(F, 9);
const hal_pin_id_t CHIP_PIN(F, 10);
const hal_pin_id_t CHIP_PIN(F, 11);
const hal_pin_id_t CHIP_PIN(F, 12);
const hal_pin_id_t CHIP_PIN(F, 13);
const hal_pin_id_t CHIP_PIN(F, 14);
const hal_pin_id_t CHIP_PIN(F, 15);

const hal_pin_id_t CHIP_PIN(G, 0);
const hal_pin_id_t CHIP_PIN(G, 1);
const hal_pin_id_t CHIP_PIN(G, 2);
const hal_pin_id_t CHIP_PIN(G, 3);
const hal_pin_id_t CHIP_PIN(G, 4);
const hal_pin_id_t CHIP_PIN(G, 5);
const hal_pin_id_t CHIP_PIN(G, 6);
const hal_pin_id_t CHIP_PIN(G, 7);
const hal_pin_id_t CHIP_PIN(G, 8);
const hal_pin_id_t CHIP_PIN(G, 9);
const hal_pin_id_t CHIP_PIN(G, 10);
const hal_pin_id_t CHIP_PIN(G, 11);
const hal_pin_id_t CHIP_PIN(G, 12);
const hal_pin_id_t CHIP_PIN(G, 13);
const hal_pin_id_t CHIP_PIN(G, 14);
const hal_pin_id_t CHIP_PIN(G, 15);

const hal_pin_id_t CHIP_PIN(H, 0);
const hal_pin_id_t CHIP_PIN(H, 1);
const hal_pin_id_t CHIP_PIN(H, 2);
const hal_pin_id_t CHIP_PIN(H, 3);
const hal_pin_id_t CHIP_PIN(H, 4);
const hal_pin_id_t CHIP_PIN(H, 5);
const hal_pin_id_t CHIP_PIN(H, 6);
const hal_pin_id_t CHIP_PIN(H, 7);
const hal_pin_id_t CHIP_PIN(H, 8);
const hal_pin_id_t CHIP_PIN(H, 9);
const hal_pin_id_t CHIP_PIN(H, 10);
const hal_pin_id_t CHIP_PIN(H, 11);
const hal_pin_id_t CHIP_PIN(H, 12);
const hal_pin_id_t CHIP_PIN(H, 13);
const hal_pin_id_t CHIP_PIN(H, 14);
const hal_pin_id_t CHIP_PIN(H, 15);

const hal_pin_id_t CHIP_PIN(I, 0);
const hal_pin_id_t CHIP_PIN(I, 1);
const hal_pin_id_t CHIP_PIN(I, 2);
const hal_pin_id_t CHIP_PIN(I, 3);
const hal_pin_id_t CHIP_PIN(I, 4);
const hal_pin_id_t CHIP_PIN(I, 5);
const hal_pin_id_t CHIP_PIN(I, 6);
const hal_pin_id_t CHIP_PIN(I, 7);
const hal_pin_id_t CHIP_PIN(I, 8);
const hal_pin_id_t CHIP_PIN(I, 9);
const hal_pin_id_t CHIP_PIN(I, 10);
const hal_pin_id_t CHIP_PIN(I, 11);
const hal_pin_id_t CHIP_PIN(I, 12);
const hal_pin_id_t CHIP_PIN(I, 13);
const hal_pin_id_t CHIP_PIN(I, 14);
const hal_pin_id_t CHIP_PIN(I, 15);

const hal_pin_id_t CHIP_PIN(J, 0);
const hal_pin_id_t CHIP_PIN(J, 1);
const hal_pin_id_t CHIP_PIN(J, 2);
const hal_pin_id_t CHIP_PIN(J, 3);
const hal_pin_id_t CHIP_PIN(J, 4);
const hal_pin_id_t CHIP_PIN(J, 5);
const hal_pin_id_t CHIP_PIN(J, 6);
const hal_pin_id_t CHIP_PIN(J, 7);
const hal_pin_id_t CHIP_PIN(J, 8);
const hal_pin_id_t CHIP_PIN(J, 9);
const hal_pin_id_t CHIP_PIN(J, 10);
const hal_pin_id_t CHIP_PIN(J, 11);
const hal_pin_id_t CHIP_PIN(J, 12);
const hal_pin_id_t CHIP_PIN(J, 13);
const hal_pin_id_t CHIP_PIN(J, 14);
const hal_pin_id_t CHIP_PIN(J, 15);

const hal_pin_id_t CHIP_PIN(K, 0);
const hal_pin_id_t CHIP_PIN(K, 1);
const hal_pin_id_t CHIP_PIN(K, 2);
const hal_pin_id_t CHIP_PIN(K, 3);
const hal_pin_id_t CHIP_PIN(K, 4);
const hal_pin_id_t CHIP_PIN(K, 5);
const hal_pin_id_t CHIP_PIN(K, 6);
const hal_pin_id_t CHIP_PIN(K, 7);
const hal_pin_id_t CHIP_PIN(K, 8);
const hal_pin_id_t CHIP_PIN(K, 9);
const hal_pin_id_t CHIP_PIN(K, 10);
const hal_pin_id_t CHIP_PIN(K, 11);
const hal_pin_id_t CHIP_PIN(K, 12);
const hal_pin_id_t CHIP_PIN(K, 13);
const hal_pin_id_t CHIP_PIN(K, 14);
const hal_pin_id_t CHIP_PIN(K, 15);

/****** Declaración de funciones privadas ********************************************************/

uint32_t     	U2STM_HAL_PIN (uint8_t);
GPIO_TypeDef *	U2STM_HAL_PORT (uint8_t);
void			U2STM_ENCENDER_RELOJ (uint8_t);

/****** Definición de funciones privadas *********************************************************/

/**------------------------------------------------------------------------------------------------
  * @brief  Traduce un PIN en formato uHAL a formato STM32
  * @param	PIN en formato uHAL
  * @retval PIN en formato STM32
  */
uint32_t        U2STM_HAL_PIN (uint8_t U_HAL_PIN) {
	if (U_HAL_PIN > 15) while (1) { /* ERROR */ };
	uint32_t STM_HAL_PIN = 1;
	STM_HAL_PIN = STM_HAL_PIN << U_HAL_PIN;
	return STM_HAL_PIN;
}

/**------------------------------------------------------------------------------------------------
  * @brief  Traduce un PUERTO en formato uHAL a formato STM32
  * @param	PUERTO en formato uHAL
  * @retval PUERTO en formato STM32
  */
GPIO_TypeDef *  U2STM_HAL_PORT (uint8_t U_HAL_PUERTO) {
	if (U_HAL_PORT > 10) while (1) { /* ERROR */ };
	GPIO_TypeDef * STM_HAL_PUERTO = NULL;
	switch (U_HAL_PUERTO) {
		case HAL_PORT_A:
			STM_HAL_PUERTO = GPIOA;
			break;
		case HAL_PORT_B:
			STM_HAL_PUERTO = GPIOB;
			break;
		case HAL_PORT_C:
			STM_HAL_PUERTO = GPIOC;
			break;
		case HAL_PORT_D:
			STM_HAL_PUERTO = GPIOD;
			break;
		case HAL_PORT_E:
			STM_HAL_PUERTO = GPIOE;
			break;
		case HAL_PORT_F:
			STM_HAL_PUERTO = GPIOF;
			break;
		case HAL_PORT_G:
			STM_HAL_PUERTO = GPIOG;
			break;
		case HAL_PORT_H:
			STM_HAL_PUERTO = GPIOH;
			break;
		case HAL_PORT_I:
			STM_HAL_PUERTO = GPIOI;
			break;
		case HAL_PORT_J:
			STM_HAL_PUERTO = GPIOJ;
			break;
		case HAL_PORT_K:
			STM_HAL_PUERTO = GPIOK;
			break;
	}
	return STM_HAL_PUERTO;
}

/**------------------------------------------------------------------------------------------------
  * @brief  Enciende el reloj correspondiente a un puerto E/S
  * @param	PUERTO
  * @retval Ninguno
  */
void 			U2STM_ENCENDER_RELOJ (uint8_t PUERTO) {
	if (PUERTO > 10) while (1) { /* ERROR */ };
	switch (PUERTO) {
			case HAL_PORT_A:
				__HAL_RCC_GPIOA_CLK_ENABLE();
				break;
			case HAL_PORT_B:
				__HAL_RCC_GPIOB_CLK_ENABLE();
				break;
			case HAL_PORT_C:
				__HAL_RCC_GPIOC_CLK_ENABLE();
				break;
			case HAL_PORT_D:
				__HAL_RCC_GPIOD_CLK_ENABLE();
				break;
			case HAL_PORT_E:
				__HAL_RCC_GPIOE_CLK_ENABLE();
				break;
			case HAL_PORT_F:
				__HAL_RCC_GPIOF_CLK_ENABLE();
				break;
			case HAL_PORT_G:
				__HAL_RCC_GPIOG_CLK_ENABLE();
				break;
			case HAL_PORT_H:
				__HAL_RCC_GPIOH_CLK_ENABLE();
				break;
			case HAL_PORT_I:
				__HAL_RCC_GPIOI_CLK_ENABLE();
				break;
			case HAL_PORT_J:
				__HAL_RCC_GPIOJ_CLK_ENABLE();
				break;
			case HAL_PORT_K:
				__HAL_RCC_GPIOK_CLK_ENABLE();
				break;
	}
}


/****** Definición de funciones públicas *********************************************************/

/**------------------------------------------------------------------------------------------------
  * @brief  Inicializa un GPIO individual
  * @param	PIN y CONFIG
  * @retval Ninguno
  * @note
  */
void uHALgpioInicializar (hal_pin_id_t PIN, hal_pin_config_s * CONFIG) {
	// Variable local
	GPIO_InitTypeDef GPIO_InitStruct = {0};	// Para inicializar gpio
											// Pin, Modo, Pull, Velocidad
											// ¿Alternate?
	// Encender reloj del puerto
	U2STM_ENCENDER_RELOJ(PIN->port);

	// Asigno valores para inicializar
	GPIO_InitStruct.Pin = U2STM_HAL_PIN(PIN->pin);
	GPIO_InitStruct.Mode = CONFIG->Modo;
	GPIO_InitStruct.Pull = CONFIG->Tirar;
	GPIO_InitStruct.Speed = CONFIG->Velocidad;

	// Inicializo
	HAL_GPIO_Init(	U2STM_HAL_PORT(PIN->port),
					&GPIO_InitStruct );
}

/**------------------------------------------------------------------------------------------------
  * @brief  Lee un PIN
  * @param	PIN
  * @retval Valor leído
  * @note
  */
bool uHALgpioLeer (hal_pin_id_t PIN) {
	GPIO_PinState Lectura = GPIO_PIN_RESET;
	Lectura = HAL_GPIO_ReadPin( U2STM_HAL_PORT (PIN->port),
								U2STM_HAL_PIN  (PIN->pin) );
	return (bool) Lectura;
}

/**------------------------------------------------------------------------------------------------
  * @brief  Escribe en un PIN
  * @param	PIN y VALOR
  * @retval Valor leído
  * @note
  */
void uHALgpioEscribir (hal_pin_id_t PIN, bool VALOR) {
	HAL_GPIO_WritePin(	U2STM_HAL_PORT (PIN->port),
						U2STM_HAL_PIN  (PIN->pin),
						VALOR );
}
