/**************************************************************************************************
* @file		Captu_RTOS.c
* @author	Guillermo Caporaletti
* @brief		Funciones de manejo de CAPTURADORA en FreeRTOS.
* @date		2024
**************************************************************************************************/


/****** Librerías (includes) *********************************************************************/

#include <math.h>
#include "Captu_RTOS.h"

/****** Definiciones privadas (macros) ***********************************************************/

#define RETARDO_CAPTURADORA_DISPONIBLE	pdMS_TO_TICKS( 50UL )
#define RETARDO_CAPTURADORA_CORRIENDO	pdMS_TO_TICKS( 3UL )

/****** Definiciones privadas de tipos (private typedef) *****************************************/


/****** Definición de datos privados *************************************************************/

SemaphoreHandle_t 	CaptuMutexAdmin;	// Esta variable global administra el manejo del semáforo
													// que administra el acceso a las funciones.
capturadora_config_s CCONFIG = {0};

/* Configuración ADC */
capturadora_config_s CAPTU_CONFIG    = {0};
entrada_config_s     ENTRADA_CONFIG  = {0};
senial_s * 				P_Senial_E1 = NULL;
senial_s * 				P_Senial_E2 = NULL;

/****** Definición de datos públicos *************************************************************/

const char * Entrada_1 = "ENTRADA 1";
const char * Entrada_2 = "ENTRADA 2";

/****** Declaración de funciones privadas ********************************************************/

void escribir_entrada(entrada_id_e);
void escribir_captu();

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

	// Semáforo Mutex para acceso concurrente a leds
	CaptuMutexAdmin = xSemaphoreCreateMutex();
	configASSERT ( NULL != CaptuMutexAdmin );

	// Chau!
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
			apli_mensaje("Comenzamos captura...",portMAX_DELAY);
			RET = true;
		} else {
			apli_mensaje("No pudimos comenzar captura.",portMAX_DELAY);
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

bool CaptuRTOS_Configurar ( capturadora_config_s * PCCONFIG, TickType_t ESPERA )
{
	bool_t RET = false;
	if(pdTRUE == xSemaphoreTake( CaptuMutexAdmin, ESPERA )) {
		if(uCapturadoraConfigurar(PCCONFIG)) {
			escribir_captu();
			//uoEscribirTxt ("MSJ Configuramos Capturadora. \n\r");
			RET = true;
		}
		xSemaphoreGive( CaptuMutexAdmin );
	}
	return RET;
}

bool CaptuRTOS_Obtener ( capturadora_config_s * PCCONFIG, TickType_t ESPERA )
{
	bool_t RET = false;
	if(pdTRUE == xSemaphoreTake( CaptuMutexAdmin, ESPERA )) {
		if(uCapturadoraObtener(PCCONFIG)) {
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
		if(uCapturadoraObtener(&CCONFIG)) {
			tomar_escritura(ESPERA);
			escribir_captu();
			escribir_entrada(ENTRADAS_TODAS);
			devolver_escritura();
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
			uoEscribirTxt ("MSJ Configuramos ENTRADA de CAPTU: ");
			uoEscribirTxtReal ("Nivel=", ECONFIG->NivelDisparo,3 );
			uoEscribirTxt ("\n");
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
			uoEscribirTxt ("MSJ CAPTU Encendimos entrada(s).\n");
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
			uoEscribirTxt ("MSJ CAPTU Apagamos entrada(s).\n");
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
	CAPTU_CONFIG.OrigenDisparo		= ENTRADA_1;
	uCapturadoraConfigurar	( &CAPTU_CONFIG );
	P_Senial_E1 = uCapturadoraLeerSenial ( ENTRADA_1 );
	P_Senial_E2 = uCapturadoraLeerSenial ( ENTRADA_2 );

	// Configuramos entradas de Capturadora

	uCapturadoraEntradaObtener    ( ENTRADA_1, &ENTRADA_CONFIG );
	ENTRADA_CONFIG.EscalaVertical = 3;
	ENTRADA_CONFIG.NivelDisparo   = 1.0;
	ENTRADA_CONFIG.FlancoDisparo  = SUBIDA;
	uCapturadoraEntradaConfigurar ( ENTRADA_1, &ENTRADA_CONFIG );
	ENTRADA_CONFIG.EscalaVertical = 3;
	ENTRADA_CONFIG.NivelDisparo   = 2.0;
	ENTRADA_CONFIG.FlancoDisparo  = SUBIDA;
	uCapturadoraEntradaConfigurar ( ENTRADA_2, &ENTRADA_CONFIG );
	uCapturadoraEntradaEncender ( ENTRADA_1 );
	uCapturadoraEntradaEncender ( ENTRADA_2 );

	/* Como la mayoría de las tareas, ciclo infinito... */
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
						apli_mensaje ("Comenzamos captura (boton).", portMAX_DELAY );
					} else {
						// No pudimos iniciar captura.
						// Esto no debería pasar, por lo que enviamos ADVERTENCIA.
						apli_alerta ("No pudimos comenzar captura.");
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
					CaptuRTOS_ImprimirSenial32();
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
	// Variables locales
	uint32_t i, Muestra, MUESTRA_ENTRADA_1, MUESTRA_ENTRADA_2, Tiempo, Disparo;
	capturadora_config_s ConfigCaptura = {0};
	entrada_config_s     ConfigEntrada = {0};
	bool_t E1_ENCENDIDA = false;
	bool_t E2_ENCENDIDA = false;

	// Precondiciones
	if ( NULL == P_Senial_E1 || NULL == P_Senial_E2 ) uoHuboErrorTxt ("en CaptuRTOS_Imprimir...");

	// Imprimimos encabezado de señal
	tomar_escritura(portMAX_DELAY);
	uoEscribirTxt ( Barra );
	uoEscribirTxt ("// Enviamos configuracion de capturadora, entradas y seniales\n");
	escribir_captu();

	if(false==uCapturadoraEntradaObtener(ENTRADA_1,&ConfigEntrada))
		apli_alerta("No se pudo imprimir E1.");
	if(true==(E1_ENCENDIDA = ConfigEntrada.Encendida))
		escribir_entrada(ENTRADA_1);
	if(false==uCapturadoraEntradaObtener(ENTRADA_2,&ConfigEntrada))
		apli_alerta("No se pudo imprimir E2.");
	if(true==(E2_ENCENDIDA = ConfigEntrada.Encendida))
		escribir_entrada(ENTRADA_2);
	if(false==(E1_ENCENDIDA||E2_ENCENDIDA)) {
		// Nada para imprimir
		devolver_escritura();
		return;
	}

	uCapturadoraObtener ( &ConfigCaptura );
	uCapturadoraEntradaObtener ( ConfigCaptura.OrigenDisparo, &ConfigEntrada );
	Disparo = P_Senial_E1->Tiempo0;
	uoEscribirTxtUint	( "MSJ CAPTU INFO PROMEDIADAS=", uCapturadoraObtenerSincronizadas() );
	uoEscribirTxtUint	( " CAPTURA_MS=", uCapturadoraObtenerTiempoCaptura() );
	uoEscribirTxtUint	( " FM=", (uint32_t) round(uCapturadoraObtenerFrecuenciaMuestreo()) );
	uoEscribirTxtUint	( " MUESTRAS=", (uint32_t) U_LARGO_CAPTURA);
	uoEscribirTxtUint	( " DISPARO=", Disparo);
	uoEscribirTxt     ( "\n");

	// Imprimimos DATOS de señales
	Tiempo = uoMilisegundos();

	uoEscribirTxt("DATOS ");
	if(E1_ENCENDIDA) uoEscribirTxt("E1 ");
	if(E2_ENCENDIDA) uoEscribirTxt("E2 {\n");

	for (i=0; i<U_LARGO_CAPTURA; i++) {
		Muestra = P_Senial_E1->Muestras_p[i];
		MUESTRA_ENTRADA_1 = ( Muestra & MASCARA_DERECHA16   );
		MUESTRA_ENTRADA_2 = ( Muestra & MASCARA_IZQUIERDA16 ) >> 16;
		//if ( (i==Disparo) && (i>0) ) uoEscribirTxt ("// ---> Disparo <---\n");
		if(E1_ENCENDIDA)						uoEscribirUint ( MUESTRA_ENTRADA_1 );	// Dato de ENTRADA 1
		if(E1_ENCENDIDA && E2_ENCENDIDA)	uoEscribirTxt  ( "\t" );						// Separación de ENTRADA
		if(E2_ENCENDIDA) 						uoEscribirUint ( MUESTRA_ENTRADA_2 );	// Dato de ENTRADA 2
		uoEscribirTxt  ( "\n" );
	}
	uoEscribirTxt("}\n");  // Fin de DATOS

	// Fin de impresión
	Tiempo = uoMilisegundos()-Tiempo;
	uoEscribirTxtUintTxt ( "MSJ Captura escrita en ", Tiempo, " ms.\n" );
	uoEscribirTxt ( Barra );
	devolver_escritura();
}

/****** Definición de funciones privadas *********************************************************/

void escribir_entrada(entrada_id_e ID)
{
	entrada_config_s ECONFIG = {0};
	if(ENTRADAS_TODAS==ID) {
		escribir_entrada(ENTRADA_1);
		escribir_entrada(ENTRADA_2);
	} else if(uCapturadoraEntradaObtener(ID,&ECONFIG)) {
		// ID de entrada
		if(ID==ENTRADA_1){
			uoEscribirTxt("MSJ CAPTU E1 INFO");
		} else if(ID==ENTRADA_2){
			uoEscribirTxt("MSJ CAPTU E2 INFO");
		} else{
			uoEscribirTxt("MSJ CAPTU ENTRADA desconocida");
		}
		// Encendida?
		if(ECONFIG.Encendida) {
			uoEscribirTxt(" ENCENDIDA=SI");
		} else {
			uoEscribirTxt(" ENCENDIDA=NO");
		}
		// Escala vertical
		uoEscribirTxtReal(" ESCALA=",ECONFIG.EscalaVertical,3);
		// Disparo
		uoEscribirTxtReal(" NIVEL=",ECONFIG.NivelDisparo,3);
		if(ECONFIG.FlancoDisparo==SUBIDA) {
			uoEscribirTxt(" FLANCO=SUBIDA");
		} else if (ECONFIG.FlancoDisparo==BAJADA) {
			uoEscribirTxt(" FLANCO=BAJADA");
		} else {
			uoEscribirTxt(" FLANCO=desconocido");
		}
		uoEscribirTxt("\n");
	}
}

void escribir_captu( void )
{
	uCapturadoraObtener(&CCONFIG);
	uoEscribirTxtReal("MSJ CAPTU INFO ESCALA=",CCONFIG.EscalaHorizontal,6);
	switch (CCONFIG.OrigenDisparo) {
	case ENTRADA_1:
		uoEscribirTxt(" ORIGEN=E1");
		break;
	case ENTRADA_2:
		uoEscribirTxt(" ORIGEN=E2");
		break;
	case ORIGEN_ASINCRONICO:
		uoEscribirTxt(" ORIGEN=ASINCRONICO");
		break;
	default:
		uoEscribirTxt("");
	}
	if (CCONFIG.ModoCaptura & CAPTURA_PROMEDIADA_4)
		uoEscribirTxt(" PROM=4");
	if (CCONFIG.ModoCaptura & CAPTURA_PROMEDIADA_16)
		uoEscribirTxt(" PROM=16");
	if (false==(CCONFIG.ModoCaptura&MASCARA_PROMEDIO))
		uoEscribirTxt(" PROM=1");
	uoEscribirTxt("\n");
}

/****************************************************************** FIN DE ARCHIVO ***************/
