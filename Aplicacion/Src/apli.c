/**************************************************************************************************
* @file		apli.c
* @author	Guillermo Caporaletti
* @brief		Módulo principal de la aplicación ISPEL
* @date		julio de 2023
*
**************************************************************************************************/

/****** Librerías (includes) *********************************************************************/

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

/* Project includes. */
#include "apli.h"

/****** Definiciones privadas (macros) ***********************************************************/

#define TAREA_PRIORIDAD_BAJA     ( tskIDLE_PRIORITY + 1UL )
#define TAREA_PRIORIDAD_MEDIA    ( tskIDLE_PRIORITY + 2UL )
#define TAREA_PRIORIDAD_ALTA     ( tskIDLE_PRIORITY + 3UL )
#define UN_MICROSEGUNDO				1

/****** Definiciones privadas de tipos (private typedef) *****************************************/


/****** Definición de datos privados *************************************************************/

static bool_t PresionadoParaEncendido = false;

/* Variables usadas para referencias las tareas. */
static TaskHandle_t PALTA_admin;
static TaskHandle_t PMEDIA_admin;
static TaskHandle_t CAPTU_admin;
static TaskHandle_t TareaTest1_m;

/****** Definición de datos públicos *************************************************************/

bool_t		EquipoEncendido = false;
boton_id_t  BotonEnPlaca, BotonEncendido;
boton_id_t	BotonGen1, BotonGen2;
boton_id_t	BotonCaptu1, BotonCaptu2, BotonCaptuDisparo;
led_id_t    LedRojoEnPlaca, LedVerdeEnPlaca, LedEncendido;
SemaphoreHandle_t MutexApliEscribir;  // Para integridad de escritura

/* Mensajes... */
const char *pcTextForMain = "// ISPEL en ejecucion.\n";
const char *Barra         = "// ===============================================\n";

/****** Declaración de funciones privadas ********************************************************/


/****** Definición de funciones privadas *********************************************************/


/****** Definición de funciones públicas *********************************************************/

/*-------------------------------------------------------------------------------------------------
 * @brief	Inicializa el módulo principal de APLICACION
 * @param	Ninguno
 * @retval	Ninguno
 */
void apli_inicializar( void )
{
	// Variables locales
	BaseType_t ret = pdFAIL;
  	// void* ptr = NULL;

	// Inicialización de capas de abstracción uHAL y uOSAL -----------------------------------------

	uoInicializar();							// Capa OSAL
	uHALinicializar();						// Capa HAL
	uHALmapInicializar( UHAL_MAP_PE5 ); // Objeto señal cuadrada MAP=PWM en pin PE5

	// Inicialización de módulos -------------------------------------------------------------------

	configASSERT( true == LedsRTOS_Inicializar()    );
	configASSERT( true == BotonesRTOS_Inicializar()	);
	configASSERT( true == CaptuRTOS_Inicializar()	);
	configASSERT( true == GenRTOS_Inicializar()	   );
	configASSERT( true == TestRTOS_Inicializar()		);
	configASSERT( true == ai_inicializar()				);

	// Inicialización y configuración de objetos ---------------------------------------------------

	configASSERT( NULL        != (MutexApliEscribir	= xSemaphoreCreateMutex()) 							);
	configASSERT( ERROR_BOTON != (BotonEnPlaca		= BotonesRTOS_InicializarBoton (U_BOTON_EP))		);
	configASSERT( ERROR_BOTON != (BotonEncendido		= BotonesRTOS_InicializarBoton (HAL_PIN_PE7))	);
	configASSERT( ERROR_BOTON != (BotonGen1  			= BotonesRTOS_InicializarBoton (HAL_PIN_PE8))	);
	configASSERT( ERROR_BOTON != (BotonGen2			= BotonesRTOS_InicializarBoton (HAL_PIN_PG9))	);
	configASSERT( ERROR_BOTON != (BotonCaptu1			= BotonesRTOS_InicializarBoton (HAL_PIN_PG14))	);
	configASSERT( ERROR_BOTON != (BotonCaptu2			= BotonesRTOS_InicializarBoton (HAL_PIN_PF15))	);
	configASSERT( ERROR_BOTON != (BotonCaptuDisparo	= BotonesRTOS_InicializarBoton (HAL_PIN_PE13))	);
	configASSERT( ERROR_LED   != (LedRojoEnPlaca  	= LedsRTOS_InicializarLed (U_LED_ROJO_EP))		);
	configASSERT( ERROR_LED   != (LedVerdeEnPlaca 	= LedsRTOS_InicializarLed (U_LED_VERDE_EP))		);
	configASSERT( ERROR_LED   != (LedEncendido    	= LedsRTOS_InicializarLed (HAL_PIN_PB11))			);

	configASSERT( LedsRTOS_ModoLed ( LedEncendido, SUSPENSION ) );
	configASSERT( LedsRTOS_EncenderLed (LedEncendido)  );
	configASSERT( LedsRTOS_ModoLed ( LedRojoEnPlaca, SUSPENSION ) );
	configASSERT( LedsRTOS_EncenderLed (LedRojoEnPlaca)  );
	uHALmapConfigurarFrecuencia( UHAL_MAP_PE5 , FREC_TESTIGO );  // Señal cuadrada testigo
	uHALmapEncender            ( UHAL_MAP_PE5 );

	// Mensaje de inicio de APLICACION -------------------------------------------------------------

	tomar_escritura(portMAX_DELAY);
	uoEscribirTxt ( "\r\n");
	uoEscribirTxt ( Barra );
	uoEscribirTxt ( pcTextForMain );
	uoEscribirTxt ( Barra );
	uoEscribirTxtUintTxt	( "MSJ Frecuencia de senial testigo (cuadrada) = ",
			  	  	  	  	  	  (uint32_t) round( uHALmapObtenerFrecuencia(UHAL_MAP_PE5)), " Hz.\n" );
	devolver_escritura();

	// Probamos leds controlados por UOSAL
	/*
  	uoLedEncender	(UOSAL_PIN_LED_VERDE_INCORPORADO);
  	uoLedEncender	(UOSAL_PIN_LED_AZUL_INCORPORADO);
  	uoLedEncender	(UOSAL_PIN_LED_ROJO_INCORPORADO);

  	uoEscribirTxt	("Espero 1 segundo con leds incorporados encendidos...\r\n");
  	uoEsperarMilis (1000);
  	uoLedApagar		(UOSAL_PIN_LED_VERDE_INCORPORADO);
  	*/

	// Creación de las tareas ---------------------------------------------------------------------

	/* Tarea PALTA para funciones rápidas cada 1 ms:
	 * - Lectura de UART
	 * - Actualización de leds
	 * - Actualización de botones */
	ret = xTaskCreate(	Tarea_PALTA,							// Puntero a la función-tarea.
								"PALTA",									// Nombre de tarea. Para desarrollo.
								(2 * configMINIMAL_STACK_SIZE),	// Tamaño de stack en palabras.
								NULL,    								// Parametros de la tarea, que no tiene acá
								TAREA_PRIORIDAD_ALTA,				// Prioridad alta.
								&PALTA_admin );						// Variable de administración de tarea.
	configASSERT( ret == pdPASS );

	/* Tarea PMEDIA para funciones cada 10 ms: */
	ret = xTaskCreate( 	Tarea_PMEDIA,
								"PMEDIA",
								(2 * configMINIMAL_STACK_SIZE),
								NULL,
								TAREA_PRIORIDAD_MEDIA,
								&PMEDIA_admin );
	configASSERT( ret == pdPASS );

	/* Tarea Test 1 */
	ret = xTaskCreate( 	Tarea_Test,
								"TEST",
								(2 * configMINIMAL_STACK_SIZE),
								NULL,
								TAREA_PRIORIDAD_MEDIA,
								&TareaTest1_m );
	configASSERT( ret == pdPASS );

	/* Tarea CAPTURADORA que exige procesamiento.
	 * En baja prioridad para que pueda ser interrumpida. */
	ret = xTaskCreate( 	Tarea_Capturadora,
								"CAPTURADORA",
								(2 * configMINIMAL_STACK_SIZE),
								NULL,
								TAREA_PRIORIDAD_BAJA,
								&CAPTU_admin );
	configASSERT( ret == pdPASS );

}

/*-------------------------------------------------------------------------------------------------
 * @brief	Tarea que se actualiza con ALTA RPIORIDAD cada 1 ms
 * @param	Ninguno
 * @descripcion
 *		 		- Lectura de UART (solo carga, no procesa).
 * 			- Actualización de leds.
 * 			- Actualización de botones.
 * Las funciones deben durar lo menos posible.
 * Utilizamos vTaskDelayUntil() porque es crítico el sincronismo.
 */
void Tarea_PALTA( void *pvParameters )
{
	/* Variables locales ------------------------------------------------------*/
	char *pcTaskName = (char *) pcTaskGetName( NULL );
	char Lectura = '\0';
	uint8_t ConteoParaBotones=0;
	TickType_t Tiempo0;

	/* Imprimir la tarea iniciada ---------------------------------------------*/
	uoEscribirTxt3 ( "MSJ ", pcTaskName, " esta ejecutandose.\n\r" );

	/* Ciclo infinito: --------------------------------------------------------*/
	Tiempo0 = xTaskGetTickCount();
	for( ;; )
	{

		/* Verificamos lectura de UART -----------------------------------------*/
		if ( (uoLeerChar(&Lectura,UN_MICROSEGUNDO)) > 0 ) {
			ai_cargar_caracter( Lectura );
			//uoEscribirTxt("-");
		}
		if ( (uoLeerChar(&Lectura,UN_MICROSEGUNDO)) > 0 ) {
			ai_cargar_caracter( Lectura );
		}
		// Lo hacemos dos veces por si la frecuencia de PC es más rápida.
		// Esperando UN_MICROSEGUNDO, toleramos discrepancia de 1000 ppm = 0,1%.

		/* Actualización de leds -----------------------------------------------*/
		LedsRTOS_ActualizarTodos ( 0 );
		// "0" porque prefiero saltear operación que esperarla

		/* Actualizamos botones ------------------------------------------------*/
		ConteoParaBotones++;
		if(ConteoParaBotones%5==0) BotonesRTOS_ActualizarTodo ( 0 );
		// Actualizamos cada 5 ms en una tarea con período de 1 ms.

		/* Retardo sincronizado ------------------------------------------------*/
		vTaskDelayUntil( &Tiempo0, PERIODO_1MS );
	}
}

/*-------------------------------------------------------------------------------------------------
 * @brief	Tarea que se actualiza con MEDIA RPIORIDAD cada 10 ms
 * @param	Ninguno
 * @descripcion:
 * 	- Procesamiento de mensajes.
 * Las funciones de esta tarea podrían demorar más de 10 ms.
 * Utilizamos vTaskDelay() porque no es tan crítico el sincronismo.
 */
void Tarea_PMEDIA ( void *pvParameters )
{
	/* Imprimir la tarea iniciada: */
	char *pcTaskName = (char *) pcTaskGetName( NULL );
	uoEscribirTxt3 ( "MSJ ", pcTaskName, " esta ejecutandose.\n\r" );

	/* Ciclo infinito: */
	for( ;; )
	{
		// Prioridad: Procesar los mensajes que estén en cola
		ai_procesar_mensajes();

		// Verifico botón ENCENDIDO
		if( BotonesRTOS_BotonFlancoPresionado(BotonEncendido) && false==EquipoEncendido ) {
			EquipoEncendido = true;
			PresionadoParaEncendido = true;
			LedsRTOS_ModoLed (LedEncendido,PLENO);
		}
		if( BotonesRTOS_BotonPresionadoLargo(BotonEncendido) && false==PresionadoParaEncendido ) {
			EquipoEncendido = false;
			LedsRTOS_ModoLed (LedEncendido,SUSPENSION);
			GenRTOS_Apagar(GENERADORES_TODOS, portMAX_DELAY);
			CaptuRTOS_EntradaApagar(ENTRADAS_TODAS, portMAX_DELAY);
		}
		if( false==BotonesRTOS_BotonPresionado(BotonEncendido)) {
			PresionadoParaEncendido = false;
		}

		// Verifico botón GEN 1
		if (BotonesRTOS_BotonFlancoPresionado( BotonGen1 ) && EquipoEncendido) {
			if (GenRTOS_EstaEncendido( GENERADOR_1, 25 * PERIODO_1MS )) {
				GenRTOS_Apagar( GENERADOR_1, 50 * PERIODO_1MS );
			} else {
				GenRTOS_Encender( GENERADOR_1, 50 * PERIODO_1MS );
			}
		}

		// Verifico botón GEN 2
		if (BotonesRTOS_BotonFlancoPresionado( BotonGen2 ) && EquipoEncendido) {
			if (GenRTOS_EstaEncendido( GENERADOR_2, 25 * PERIODO_1MS )) {
				GenRTOS_Apagar( GENERADOR_2, 50 * PERIODO_1MS );
			} else {
				GenRTOS_Encender( GENERADOR_2, 50 * PERIODO_1MS );
			}
		}

		// Verifico botón CAPTU 1
		if (BotonesRTOS_BotonFlancoPresionado( BotonCaptu1 ) && EquipoEncendido) {
			if (CaptuRTOS_EntradaEstaEncendida( ENTRADA_1, 25 * PERIODO_1MS )) {
				CaptuRTOS_EntradaApagar( ENTRADA_1, 50 * PERIODO_1MS );
			} else {
				CaptuRTOS_EntradaEncender( ENTRADA_1, 50 * PERIODO_1MS );
			}
		}

		// Verifico botón CAPTU 2
		if (BotonesRTOS_BotonFlancoPresionado( BotonCaptu2 )  && EquipoEncendido) {
			if (CaptuRTOS_EntradaEstaEncendida( ENTRADA_2, 25 * PERIODO_1MS )) {
				CaptuRTOS_EntradaApagar( ENTRADA_2, 50 * PERIODO_1MS );
			} else {
				CaptuRTOS_EntradaEncender( ENTRADA_2, 50 * PERIODO_1MS );
			}
		}

		// FIN DE CICLO
		//apli_separador(".");
		apli_latido();
		vTaskDelay( 25 * PERIODO_1MS );
	}
}

/**************************************************************************************************
*   FIN DE ARCHIVO
**************************************************************************************************/
