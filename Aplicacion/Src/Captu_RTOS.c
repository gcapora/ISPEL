/**************************************************************************************************
* @file		....c
* @author	Guillermo Caporaletti
* @brief
* @date
*
**************************************************************************************************/


/****** Librerías (includes) *********************************************************************/

#include "Captu_RTOS.h"

/****** Definiciones privadas (macros) ***********************************************************/


/****** Definiciones privadas de tipos (private typedef) *****************************************/


/****** Definición de datos privados *************************************************************/

SemaphoreHandle_t CaptuMutexAdmin;	// Esta variable global administra el manejo del semáforo
												// que administra el acceso a las funciones.

/****** Definición de datos públicos *************************************************************/


/****** Declaración de funciones privadas ********************************************************/


/****** Definición de funciones privadas *********************************************************/


/****** Definición de funciones públicas *********************************************************/

/*-------------------------------------------------------------------------------------------------
 * @brief  Inicializa todos los leds a utilizar
 * @param	Ninguno
 * @retval true, si no hubo problemas
 */
bool CapturadoraRTOS_Inicializar (void)
{
	bool_t RET = true;

	// Inicializamos interfase uCapturadora
	if ( !uCapturadoraInicializar() ) RET = false;

	// Creamos un semáforo Mutex para el acceso concurrente a leds
	CaptuMutexAdmin = xSemaphoreCreateMutex();
	configASSERT ( NULL != CaptuMutexAdmin );

	// Terminada la inicialización...
	return RET;
}

/*-------------------------------------------------------------------------------------------------
 * @brief	Tarea que sigue el hilo de la Capturadora
 * @param	Ninguno
 */
void Tarea_Capturadora( void *pvParameters )
{
	/* Variables locales */
	char *pcTaskName = (char *) pcTaskGetName( NULL );
	bool_t Capturando = false;
   TickType_t Tiempo0;

	/* Imprimir la tarea inicializada: */
	uoEscribirTxtTxt ( pcTaskName, " esta ejecutandose.\n\r" );

	/* Como la mayoría de las tareas, ciclo infinito... */
	Tiempo0 = xTaskGetTickCount();
	for( ;; )
	{

		// Verificamos lectura de boton
		if ( true == BotonesRTOS_BotonFlancoPresionado ( BotonEnPlaca ) ){
			if (uCapturadoraIniciar ()) {
				uoEscribirTxt ("Lanzamos captura... \n\r");
				Capturando = true;
			} else {
				uoEscribirTxt ("No pudimos lanzar captura. :-( \n\r");
			}
		}

		// Revisamos capturadora
		uCapturadoraActualizar();

		// Verificamos si debo imprimir una señal
		if ( uCapturadoraSenialCargada() ){
			  //ImprimirSenial32_main();
			  uoEscribirTxt ( Barra );
			  //uoLedApagar ( UOSAL_PIN_LED_VERDE_INCORPORADO );
			  ImprimirSenial32_main();
			  uoEscribirTxt ( Barra );
			  Capturando = false;
		}

		// Retardo variable
		if (Capturando) {
			vTaskDelayUntil( &Tiempo0, 1UL );
			//vTaskDelay( 1UL );
		} else {
			vTaskDelayUntil( &Tiempo0, 25UL );
			//vTaskDelay( 20UL );
		}
	}
}

/****************************************************************** FIN DE ARCHIVO ***************/
