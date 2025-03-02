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

SemaphoreHandle_t 	CaptuMutexAdmin;			// Manejo del semáforo para acceso a las funciones.
capturadora_config_s CCONFIG = {0};				// Config general de Capturadora.
static led_id_t 		LedCaptu1, LedCaptu2;	// Leds de entradas
static led_id_t 		LedCaptuDisparo;			// Leds del disparo de Capturadora

/* Configuración ADC */
capturadora_config_s CAPTU_CONFIG    = {0};
entrada_config_s     ENTRADA_CONFIG  = {0};
senial_s * 				P_Senial_E1 = NULL;
senial_s * 				P_Senial_E2 = NULL;

/****** Definición de datos públicos *************************************************************/

const char * Entrada_1 = "ENTRADA 1";
const char * Entrada_2 = "ENTRADA 2";
extern led_id_t LedVerdeEnPlaca;

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

	// Leds
	configASSERT( ERROR_LED   != (LedCaptu1 = LedsRTOS_InicializarLed ( HAL_PIN_PE0 )) );
	configASSERT( ERROR_LED   != (LedCaptu2 = LedsRTOS_InicializarLed ( HAL_PIN_PB0 )) );
	configASSERT( ERROR_LED   != (LedCaptuDisparo = LedsRTOS_InicializarLed ( HAL_PIN_PE2 )) );
	configASSERT( LedsRTOS_ModoLed ( LedCaptu1, SUSPENSION ) );
	configASSERT( LedsRTOS_ModoLed ( LedCaptu2, SUSPENSION ) );
	configASSERT( LedsRTOS_ModoLed ( LedCaptuDisparo, TITILANTE ) );

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
			LedsRTOS_EncenderLed ( LedCaptuDisparo );
			LedsRTOS_ModoLed 		( LedCaptu1, TITILANTE );
			LedsRTOS_ModoLed 		( LedCaptu2, TITILANTE );
			//LedsRTOS_EncenderLed ( LedVerdeEnPlaca );
			//uoLedEncender ( UOSAL_PIN_LED_VERDE_INCORPORADO );
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
			LedsRTOS_ApagarLed ( LedCaptuDisparo );
			LedsRTOS_ModoLed ( LedCaptu1, SUSPENSION );
			LedsRTOS_ModoLed ( LedCaptu2, SUSPENSION );
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

bool	CaptuRTOS_EscribirConfiguracionEntrada ( entrada_id_e ID, TickType_t ESPERA)
{
	bool_t RET = false;
	if(pdTRUE == xSemaphoreTake( CaptuMutexAdmin, ESPERA )) {
		RET = true;
		tomar_escritura(ESPERA);
		escribir_entrada(ID);
		devolver_escritura();
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

bool CaptuRTOS_EntradaEstaEncendida ( entrada_id_e ID, TickType_t ESPERA )
{
	bool_t RET = false;
	entrada_config_s ECONFIG;
	if(pdTRUE == xSemaphoreTake( CaptuMutexAdmin, ESPERA )) {
		if(uCapturadoraEntradaObtener(ID, &ECONFIG)) {
			if (true==ECONFIG.Encendida) {
				RET = true;
			}
		}
		xSemaphoreGive( CaptuMutexAdmin );
	}
	return RET;
}

bool CaptuRTOS_EntradaEncender ( entrada_id_e ID, TickType_t ESPERA )
{
	bool_t RET = false;
	if( false==EquipoEncendido ) return false;
	if(pdTRUE == xSemaphoreTake( CaptuMutexAdmin, ESPERA )) {
		if(uCapturadoraEntradaEncender(ID)) {
			if ( ENTRADA_1==ID || ENTRADAS_TODAS==ID ) LedsRTOS_EncenderLed ( LedCaptu1 );
			if ( ENTRADA_2==ID || ENTRADAS_TODAS==ID ) LedsRTOS_EncenderLed ( LedCaptu2 );
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
			if ( ENTRADA_1==ID || ENTRADAS_TODAS==ID ) LedsRTOS_ApagarLed ( LedCaptu1 );
			if ( ENTRADA_2==ID || ENTRADAS_TODAS==ID ) LedsRTOS_ApagarLed ( LedCaptu2 );
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
			if( EquipoEncendido                                       &&
				 (BotonesRTOS_BotonFlancoPresionado(BotonEnPlaca)   ||
				  BotonesRTOS_BotonFlancoPresionado(BotonCaptuDisparo))  ){
				// Se presionó un botón para iniciar captura.
				CaptuRTOS_Comenzar ( RETARDO_CAPTURADORA_DISPONIBLE );
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
					LedsRTOS_ApagarLed	( LedCaptuDisparo );
					LedsRTOS_ModoLed 		( LedCaptu1, SUSPENSION );
					LedsRTOS_ModoLed		( LedCaptu2, SUSPENSION );
					//LedsRTOS_ApagarLed	( LedVerdeEnPlaca );
				}
				xSemaphoreGive( CaptuMutexAdmin );
				// Sin retardo.
			} // <-- Fin de estado capturando --------------------------------------------------------
		} // <----- Fin de evaluación de estado de Capturadora
	} // <-------- Fin ciclo infinito de hilo de tarea
} // <----------- Fin Tarea_Capturadora()

/****** Definición de funciones privadas *********************************************************/

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

	// Imprimimos configuraciones de entradas (si corresponde)
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

	// Imprimimos configuración de capturadora
	escribir_captu();
	uCapturadoraObtener ( &ConfigCaptura );
	uCapturadoraEntradaObtener ( ConfigCaptura.OrigenDisparo, &ConfigEntrada );
	Disparo = P_Senial_E1->Tiempo0;
	uoEscribirTxtUint	( "CAPTU INFO PROMEDIADAS=", uCapturadoraObtenerSincronizadas() );
	uoEscribirTxtUint	( " CAPTURA_MS=", uCapturadoraObtenerTiempoCaptura() );
	uoEscribirTxtUint	( " FM=", (uint32_t) round(uCapturadoraObtenerFrecuenciaMuestreo()) );
	uoEscribirTxtUint	( " MUESTRAS=", (uint32_t) U_LARGO_CAPTURA);
	uoEscribirTxtUint	( " DISPARO=", Disparo);
	uoEscribirTxt     ( "\n");

	// Imprimimos DATOS de señales
	Tiempo = uoMilisegundos();

	uoEscribirTxt("DATOS ");
	if(E1_ENCENDIDA) uoEscribirTxt("E1 ");
	if(E2_ENCENDIDA) uoEscribirTxt("E2 ");
	if (E1_ENCENDIDA || E2_ENCENDIDA) uoEscribirTxt("{\n");

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

void escribir_entrada(entrada_id_e ID)
{
	entrada_config_s ECONFIG = {0};
	if(ENTRADAS_TODAS==ID) {
		escribir_entrada(ENTRADA_1);
		escribir_entrada(ENTRADA_2);
	} else if(uCapturadoraEntradaObtener(ID,&ECONFIG)) {
		// ID de entrada
		if(ID==ENTRADA_1){
			uoEscribirTxt("CAPTU E1 CONFIGURADA");
		} else if(ID==ENTRADA_2){
			uoEscribirTxt("CAPTU E2 CONFIGURADA");
		} else{
			uoEscribirTxt("MSJ CAPTU ENTRADA desconocida.\n");
			return;
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
	uoEscribirTxtReal("CAPTU CONFIGURADA ESCALA=",CCONFIG.EscalaHorizontal,6);
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
