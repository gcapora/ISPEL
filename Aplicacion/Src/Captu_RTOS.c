/**************************************************************************************************
* @file		....c
* @author	Guillermo Caporaletti
* @brief
* @date
*
**************************************************************************************************/


/****** Librerías (includes) *********************************************************************/

#include <math.h>
#include "Captu_RTOS.h"

/****** Definiciones privadas (macros) ***********************************************************/

#define RETARDO_CAPTURADORA_DISPONIBLE	pdMS_TO_TICKS( 25UL )
#define RETARDO_CAPTURADORA_CORRIENDO	pdMS_TO_TICKS( 3UL )

/****** Definiciones privadas de tipos (private typedef) *****************************************/


/****** Definición de datos privados *************************************************************/

SemaphoreHandle_t CaptuMutexAdmin;	// Esta variable global administra el manejo del semáforo
												// que administra el acceso a las funciones.

/* Configuración ADC */
capturadora_config_s CAPTU_CONFIG    = {0};
entrada_config_s     ENTRADA_CONFIG  = {0};
senial_s * 				P_Senial_E1 = NULL;
senial_s * 				P_Senial_E2 = NULL;

/****** Definición de datos públicos *************************************************************/

const char * Entrada_1 = "ENTRADA 1";
const char * Entrada_2 = "ENTRADA 2";

/****** Declaración de funciones privadas ********************************************************/


/****** Definición de funciones privadas *********************************************************/


/****** Definición de funciones públicas *********************************************************/

/*=================================================================================================
* @brief		Inicializa recursos de CaptuRTOS
* @param		Ninguno
* @retval	true, si no hubo problemas
*/
bool CaptuRTOS_Inicializar (void)
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

/*=================================================================================================
* @brief		Pedido de comienzo de captura
* @param		ticks que puede esperar.
* @retval	true, si logró comenzar.
*/
bool	CaptuRTOS_Comenzar ( TickType_t ESPERA )
{
	bool_t RET = false;
	if(pdTRUE == xSemaphoreTake( CaptuMutexAdmin, ESPERA )) {
		if(uCapturadoraIniciar()) {
			uoEscribirTxt ("MSJ Comenzamos captura (tarea)... \n\r");
			RET = true;
		}
		xSemaphoreGive( CaptuMutexAdmin );
	}
	return RET;
}

bool CaptuRTOS_Parar ( TickType_t ESPERA )
{
	bool_t RET = false;
	if(pdTRUE == xSemaphoreTake( CaptuMutexAdmin, ESPERA )) {
		if(uCapturadoraParar()) {
			uoEscribirTxt ("MSJ Paramos captura (tarea). \n\r");
			RET = true;
		}
		xSemaphoreGive( CaptuMutexAdmin );
	}
	return RET;
}

bool CaptuRTOS_Configurar ( capturadora_config_s * CCONFIG, TickType_t ESPERA )
{
	bool_t RET = false;
	if(pdTRUE == xSemaphoreTake( CaptuMutexAdmin, ESPERA )) {
		if(uCapturadoraConfigurar(CCONFIG)) {
			uoEscribirTxt ("MSJ Configuramos Capturadora. \n\r");
			RET = true;
		}
		xSemaphoreGive( CaptuMutexAdmin );
	}
	return RET;
}

bool CaptuRTOS_Obtener ( capturadora_config_s * CCONFIG, TickType_t ESPERA )
{
	bool_t RET = false;
	if(pdTRUE == xSemaphoreTake( CaptuMutexAdmin, ESPERA )) {
		if(uCapturadoraObtener(CCONFIG)) {
			RET = true;
		}
		xSemaphoreGive( CaptuMutexAdmin );
	}
	return RET;
}

bool CaptuRTOS_EscribirConfiguraciones ( TickType_t ESPERA )
{
	bool_t RET = false;
	if(pdTRUE == xSemaphoreTake( CaptuMutexAdmin, ESPERA )) {
		if(uCapturadoraParar()) {
			uoEscribirTxt ("MSJ (aqui debemos escribir la configuracion) \n\r");
			RET = true;
		}
		xSemaphoreGive( CaptuMutexAdmin );
	}
	return RET;
}

bool CaptuRTOS_EntradaConfigurar ( entrada_id_e ID, entrada_config_s * ECONFIG, TickType_t ESPERA )
{
	bool_t RET = false;
	if(pdTRUE == xSemaphoreTake( CaptuMutexAdmin, ESPERA )) {
		if(uCapturadoraEntradaConfigurar(ID, ECONFIG)) {
			uoEscribirTxt ("MSJ Configuramos ENTRADA de CAPTURADORA. \n\r");
			RET = true;
		}
		xSemaphoreGive( CaptuMutexAdmin );
	}
	return RET;
}

bool CaptuRTOS_EntradaObtener ( entrada_id_e ID, entrada_config_s * ECONFIG, TickType_t ESPERA )
{
	bool_t RET = false;
	if(pdTRUE == xSemaphoreTake( CaptuMutexAdmin, ESPERA )) {
		if(uCapturadoraEntradaObtener(ID, ECONFIG)) {
			RET = true;
		}
		xSemaphoreGive( CaptuMutexAdmin );
	}
	return RET;
}

bool CaptuRTOS_EntradaEncender ( entrada_id_e ID, TickType_t ESPERA )
{
	bool_t RET = false;
	if(pdTRUE == xSemaphoreTake( CaptuMutexAdmin, ESPERA )) {
		if(uCapturadoraEntradaEncender(ID)) {
			uoEscribirTxt ("MSJ Encendimos Entrada de Capturadora. \n\r");
			RET = true;
		}
		xSemaphoreGive( CaptuMutexAdmin );
	}
	return RET;
}

bool CaptuRTOS_EntradaApagar ( entrada_id_e ID, TickType_t ESPERA )
{
	bool_t RET = false;
	if(pdTRUE == xSemaphoreTake( CaptuMutexAdmin, ESPERA )) {
		if(uCapturadoraEntradaApagar(ID)) {
			uoEscribirTxt ("MSJ Apagamos Entrada de CAPTURADORA. \n\r");
			RET = true;
		}
		xSemaphoreGive( CaptuMutexAdmin );
	}
	return RET;
}

/*=================================================================================================
 * @brief	Tarea que sigue el hilo de la Capturadora
 * @param	Ninguno
 */
void Tarea_Capturadora( void *pvParameters )
{
	/* Variables locales */
	char *pcTaskName = (char *) pcTaskGetName( NULL );
	//TickType_t Tiempo0;

	/* Imprimir la tarea inicializada: */
	uoEscribirTxt3 ( "MSJ ", pcTaskName, " esta ejecutandose.\n\r" );

	// Configura Capturadora

	uCapturadoraObtener		( &CAPTU_CONFIG );
	CAPTU_CONFIG.EscalaHorizontal = 2/FREC_TESTIGO;
	CAPTU_CONFIG.ModoCaptura      = CAPTURA_PROMEDIADA_16;
	uCapturadoraConfigurar	( &CAPTU_CONFIG );
	P_Senial_E1 = uCapturadoraSenialObtener ( ENTRADA_1 );
	P_Senial_E2 = uCapturadoraSenialObtener ( ENTRADA_2 );

	// Configuramos entradas de Capturadora

	uCapturadoraEntradaObtener    ( ENTRADA_1, &ENTRADA_CONFIG );
	ENTRADA_CONFIG.EscalaVertical = 3;
	ENTRADA_CONFIG.NivelDisparo   = 1.5;
	ENTRADA_CONFIG.FlancoDisparo  = SUBIDA;
	uCapturadoraEntradaConfigurar ( ENTRADA_1, &ENTRADA_CONFIG );
	ENTRADA_CONFIG.EscalaVertical = 3;
	ENTRADA_CONFIG.NivelDisparo   = 2.5;
	ENTRADA_CONFIG.FlancoDisparo  = SUBIDA;
	uCapturadoraEntradaConfigurar ( ENTRADA_2, &ENTRADA_CONFIG );
	uCapturadoraEntradaEncender ( ENTRADA_1 );
	uCapturadoraEntradaEncender ( ENTRADA_2 );

	/* Como la mayoría de las tareas, ciclo infinito... */
	//Tiempo0 = xTaskGetTickCount();
	for( ;; )
	{
		/* ------------------------------------------------------------------------------------------
		* DOS ESTADOS POSIBLES:
		* 1) Captu DISPONIBLE, a la espera de lanzar captura.
		* 2) Captu capturando, procesando o imprimiendo.
		*/

		/* 1) ESTADO DISPONIBLE -------------------------------------------------------------------*/
		if(uCapturadoraEstaDisponible()) {
			// Verificamos lectura de boton.
			if ( true == BotonesRTOS_BotonFlancoPresionado ( BotonEnPlaca ) ){
				// Se presionó un botón para iniciar captura.
				if(pdTRUE == xSemaphoreTake( CaptuMutexAdmin, RETARDO_CAPTURADORA_DISPONIBLE )) {
					// Intentamos iniciar captura.
					if(uCapturadoraIniciar()) {
						// Logramos iniciar captura. MENSAJE.
						uoLedEncender ( UOSAL_PIN_LED_VERDE_INCORPORADO );
						uoEscribirTxt ("MSJ Comenzamos captura (boton)... \n\r");
					} else {
						// No pudimos iniciar captura.
						// Esto no debería pasar, por lo que enviamos ADVERTENCIA.
						uoEscribirTxt ("ADVERTENCIA No pudimos comenzar captura.\n\r");
					}
					xSemaphoreGive( CaptuMutexAdmin );
				}
				// Sin retardo.
			} else {
				// Retardo largo porque estamos inactivos
				vTaskDelay( RETARDO_CAPTURADORA_DISPONIBLE );
			}

		/* 2) ESTADO CAPTURANDO -------------------------------------------------------------------*/
		} else {
			if(pdTRUE == xSemaphoreTake( CaptuMutexAdmin, RETARDO_CAPTURADORA_CORRIENDO )) {
				// Revisamos capturadora
				uCapturadoraActualizar();
				// Verificamos si debo imprimir una señal
				if ( uCapturadoraSenialCargada() ){
					uoEscribirTxt ( Barra );
					CaptuRTOS_ImprimirSenial32();  // Esto debería adecuarse a un formato luego.
					uoEscribirTxt ( Barra );
					uoLedApagar ( UOSAL_PIN_LED_VERDE_INCORPORADO );
				}
				xSemaphoreGive( CaptuMutexAdmin );
				// Sin retardo.
			} // <-- Fin de estado capturando --------------------------------------------------------
		} // <----- Fin de evaluación de estado de Capturadora
	} // <-------- Fin ciclo infinito de hilo de tarea
} // <----------- Fin Tarea_Capturadora()


void CaptuRTOS_ImprimirSenial32 (void)
{
	// Variables locales:
	uint32_t i, Muestra, MUESTRA_ENTRADA_1, MUESTRA_ENTRADA_2, Disparo, Tiempo;
	capturadora_config_s ConfigCaptura = {0};
	entrada_config_s     ConfigEntrada = {0};

	// Precondiciones
	if ( NULL == P_Senial_E1 || NULL == P_Senial_E2 ) uoHuboErrorTxt ("en Imprimir... ppal.");

	// Asignaciones iniciales
	Disparo = P_Senial_E1->ReferenciaT0;

	// Escribimos última muestra:
	uoEscribirTxt ("Senial cargada:");
	uoEscribirTxt ("\n\rENT_1 \tENT_2\n\r");

	Tiempo = uoMilisegundos();

	for (i=0; i<U_LARGO_CAPTURA; i++) {

		Muestra = P_Senial_E1->Muestras_p[i];
		MUESTRA_ENTRADA_1 = ( Muestra & MASCARA_DERECHA16   );
		MUESTRA_ENTRADA_2 = ( Muestra & MASCARA_IZQUIERDA16 ) >> 16;

		if ( (i==Disparo) && (i>0) ) uoEscribirTxt ("---> Disparo <---\n\r");

		uoEscribirUint ( MUESTRA_ENTRADA_1 );	// Dato de ENTRADA 1
		uoEscribirTxt  ( "\t" );		// Tabulación
		uoEscribirUint ( MUESTRA_ENTRADA_2 );	// Dato de ENTRADA 2
		//uEscribirTxt  ( "\t" );		// Tabulación
		//uEscribirUint ( CantidadProcesadas12[i] );	// Dato de ENTRADA 2
		uoEscribirTxt  ( "\n\r" );
	}

	uCapturadoraObtener ( &ConfigCaptura );
	uCapturadoraEntradaObtener ( ConfigCaptura.OrigenDisparo, &ConfigEntrada );

	uoEscribirTxtUint	( "Capturas sincronizadas \t= ", uCapturadoraLeerSincronizadas() );
	uoEscribirTxt		( "\n\r" );
	uoEscribirTxtUint	( "Tiempo de captura \t= ", uCapturadoraLeerTiempoCaptura() );
	uoEscribirTxt     	( " ms\n\r" );
	uoEscribirTxtUint	( "Frecuencia muestreo \t= ", (uint32_t) round(uCapturadoraLeerFrecuenciaMuestreo()) );
	uoEscribirTxt     	( " Hz\n\r");

	switch (ConfigCaptura.OrigenDisparo)
	{
		case ENTRADA_1:
			uoEscribirTxt		( "Origen de disparo\t= ENTRADA 1\n\r" );
			uoEscribirTxtUint	( "Nivel (10V) \t\t= ", (uint32_t) (10.0*ConfigEntrada.NivelDisparo) );
		   uoEscribirTxt		( "\n\r" );
		   break;
		case ENTRADA_2:
			uoEscribirTxt 		( "Origen de disparo\t= ENTRADA 2\n\r" );
			uoEscribirTxtUint	( "Nivel (10V) \t\t= ", (uint32_t) (10.0*ConfigEntrada.NivelDisparo) );
		   uoEscribirTxt		( "\n\r" );
			break;
		case ORIGEN_ASINCRONICO:
			uoEscribirTxt 		( "Origen de disparo\t= ASINCRONICO\n\r" );
			break;
		default:
			// nada
			break;
	  }

	uCapturadoraEntradaObtener ( ENTRADA_1, &ConfigEntrada );
	uoEscribirTxtUint ( "Escala Entrada 1\t= ", (uint32_t) (uint32_t) round(10.0*ConfigEntrada.EscalaVertical) );
	uoEscribirTxt     ( "\n\r" );
	uCapturadoraEntradaObtener ( ENTRADA_2, &ConfigEntrada );
	uoEscribirTxtUint ( "Escala Entrada 2\t= ", (uint32_t) (uint32_t) round(10.0*ConfigEntrada.EscalaVertical) );
	uoEscribirTxt     ( "\n\r" );

	Tiempo = uoMilisegundos()-Tiempo;
	uoEscribirTxtUint ( "( Escrito en ", Tiempo );
	uoEscribirTxt     ( " ms. )\n\r" );

}

/****************************************************************** FIN DE ARCHIVO ***************/
