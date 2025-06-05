/**************************************************************************************************
* @file	   apli.c
* @author  Guillermo Caporaletti
* @brief   Módulo principal de la aplicación ISPEL, invocado desde "Core\Src\main.c".
* @date	   Junio de 2025
* @version v1.1
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

#define TAREA_PRIORIDAD_BAJA	( tskIDLE_PRIORITY + 1UL )
#define TAREA_PRIORIDAD_MEDIA	( tskIDLE_PRIORITY + 2UL )
#define TAREA_PRIORIDAD_ALTA	( tskIDLE_PRIORITY + 3UL )
#define UN_MICROSEGUNDO			1

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
const char *pcTextForMain = "// ISPEL en ejecucion.\r\n";
const char *Barra         = "// ===============================================\r\n";

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

	// Inicialización de capas y módulos ----------------------------------------------------------

	uoInicializar();									// Capa OSAL
	uHALinicializar();									// Capa HAL

	configASSERT( true == LedsRTOS_Inicializar() );		// Manejo de leds en RTOS (a nivel de aplicación)
	configASSERT( true == BotonesRTOS_Inicializar()	);	// Manejo de botones en RTOS
	configASSERT( true == CaptuRTOS_Inicializar() );	// Capturadora en RTOS
	configASSERT( true == GenRTOS_Inicializar() );		// Generador en RTOS
	configASSERT( true == TestRTOS_Inicializar() );		// Módulo de testeo
	configASSERT( true == ai_inicializar() );			// Interpretador de mensajes

	// Inicialización y configuración de objetos ---------------------------------------------------

	uHALmapInicializar( UHAL_MAP_PE5 ); // Objeto señal cuadrada MAP=PWM en pin PE5
	configASSERT( NULL        != (MutexApliEscribir	= xSemaphoreCreateMutex()) 							);
	configASSERT( ERROR_BOTON != (BotonEnPlaca		= BotonesRTOS_InicializarBoton (U_BOTON_EP))		);
	configASSERT( ERROR_BOTON != (BotonEncendido	= BotonesRTOS_InicializarBoton (PIN_BOTON_ENCENDIDO)) );
	configASSERT( ERROR_BOTON != (BotonGen1  		= BotonesRTOS_InicializarBoton (PIN_BOTON_GEN_SAL_1)) );
	configASSERT( ERROR_BOTON != (BotonGen2			= BotonesRTOS_InicializarBoton (PIN_BOTON_GEN_SAL_2)) );
	configASSERT( ERROR_BOTON != (BotonCaptu1		= BotonesRTOS_InicializarBoton (PIN_BOTON_CAPTU_ENT_1)) );
	configASSERT( ERROR_BOTON != (BotonCaptu2		= BotonesRTOS_InicializarBoton (PIN_BOTON_CAPTU_ENT_2)) );
	configASSERT( ERROR_BOTON != (BotonCaptuDisparo	= BotonesRTOS_InicializarBoton (PIN_BOTON_CAPTU_DISPARO)) );
	configASSERT( ERROR_LED   != (LedRojoEnPlaca  	= LedsRTOS_InicializarLed (U_LED_ROJO_EP)) );
	configASSERT( ERROR_LED   != (LedEncendido    	= LedsRTOS_InicializarLed (HAL_PIN_PE15)) );
	//configASSERT( ERROR_LED   != (LedVerdeEnPlaca 	= LedsRTOS_InicializarLed (U_LED_VERDE_EP))		);

	configASSERT( LedsRTOS_ModoLed		( LedEncendido, SUSPENSION ) );
	configASSERT( LedsRTOS_EncenderLed	( LedEncendido )  );
	configASSERT( LedsRTOS_ModoLed		( LedRojoEnPlaca, SUSPENSION ) );
	configASSERT( LedsRTOS_EncenderLed	( LedRojoEnPlaca )  );
	uHALmapConfigurarFrecuencia	( UHAL_MAP_PE5 , FREC_TESTIGO );  // Señal cuadrada testigo
	uHALmapEncender				( UHAL_MAP_PE5 );

	// Mensaje de inicio de APLICACION -------------------------------------------------------------

	tomar_escritura(portMAX_DELAY);
	uoEscribirTxt ( "\r\n");
	uoEscribirTxt ( Barra );
	uoEscribirTxt ( pcTextForMain );
	uoEscribirTxt ( Barra );
	uoEscribirTxtUintTxt ( "MSJ Frecuencia de senial testigo (cuadrada) = ", (uint32_t) round( uHALmapObtenerFrecuencia(UHAL_MAP_PE5)), " Hz.\r\n" );
	devolver_escritura();

	// Creación de las tareas ---------------------------------------------------------------------

	/* Tarea PALTA para funciones rápidas cada 1 ms:
	 * - Lectura de UART
	 * - Actualización de leds
	 * - Actualización de botones */
	ret = xTaskCreate(	Tarea_PALTA,					// Puntero a la función-tarea.
						"PALTA",						// Nombre de tarea. Para desarrollo.
						(2 * configMINIMAL_STACK_SIZE),	// Tamaño de stack en palabras.
						NULL,    						// Parametros de la tarea, que no tiene acá
						TAREA_PRIORIDAD_ALTA,			// Prioridad alta.
						&PALTA_admin );					// Variable de administración de tarea.
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
	uoEscribirTxt3 ( "MSJ ", pcTaskName, " esta ejecutandose.\r\n" );

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
	uoEscribirTxt3 ( "MSJ ", pcTaskName, " esta ejecutandose.\r\n" );

	/* Ciclo infinito: */
	for( ;; )
	{
		// Prioridad: Procesar los mensajes que estén en cola
		ai_procesar_mensajes();

		// Verifico botón ENCENDIDO
		if(		( BotonesRTOS_BotonFlancoPresionado(BotonEncendido) ||
				BotonesRTOS_BotonFlancoPresionado(BotonEnPlaca) ) &&
				false==EquipoEncendido ) {
			// Equipo pasa de estado ESPERA a ENCENDIDO
			EquipoEncendido = true;
			PresionadoParaEncendido = true;
			LedsRTOS_ModoLed (LedEncendido,PLENO);
			LedsRTOS_ModoLed (LedRojoEnPlaca,TITILANTE);
			apli_mensaje("Equipo ISPEL en estado ENCENDIDO.", UN_SEGUNDO);
		}
		if( 	( BotonesRTOS_BotonPresionadoLargo(BotonEncendido) ||
				BotonesRTOS_BotonPresionadoLargo(BotonEnPlaca) ) &&
				false==PresionadoParaEncendido ) {
			// Equipo pasa de estado ENCENDIDO a ESPERA
			EquipoEncendido = false;
			LedsRTOS_ModoLed (LedEncendido,SUSPENSION);
			LedsRTOS_ModoLed (LedRojoEnPlaca,SUSPENSION);
			GenRTOS_Apagar(GENERADORES_TODOS, portMAX_DELAY);
			CaptuRTOS_EntradaApagar(ENTRADAS_TODAS, portMAX_DELAY);
			apli_mensaje("Equipo ISPEL en estado ESPERA.", UN_SEGUNDO);
		}
		if(		false==BotonesRTOS_BotonPresionado(BotonEncendido) &&
				false==BotonesRTOS_BotonPresionado(BotonEnPlaca) ) {
			// Soltamos el boton que cambió el estado del equipo
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
