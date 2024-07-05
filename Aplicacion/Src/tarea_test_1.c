/**************************************************************************************************
* @file		tarea_test_1.c
* @author	Guillermo Caporaletti
* @brief	Tarea para testear aplicación.
* @date		julio de 2023
*
**************************************************************************************************/


/****** Librerías (includes) *********************************************************************/

#include "tarea_test_1.h"

/****** Definiciones privadas (macros) ***********************************************************/

#define DELTAT_TEST			pdMS_TO_TICKS( 500UL )
#define ESPERA_CAPTU			pdMS_TO_TICKS( 5UL )

/****** Definiciones privadas de tipos (private typedef) *****************************************/


/****** Definición de datos públicos *************************************************************/

led_id_t    LedVerdeEnPlaca, LedRojoEnPlaca, LedAzulEnPlaca;

/****** Definición de datos privados *************************************************************/

uint32_t senial1[60] = {	0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4000,
									4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000,
									3500, 3000, 2500, 2000, 1500, 1000, 500, 0, 0, 0,
									0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
									0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
									0, 0, 0, 0, 0, 0, 0, 0, 0, 0	};

uint32_t senial2[60] = {	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
									0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4000,
									4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000,
									3500, 3000, 2500, 2000, 1500, 1000, 500, 0, 0, 0,
									0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
									0, 0, 0, 0, 0, 0, 0, 0, 0, 0	};

double   Frecuencia = 0;
gen_conf_s GenConfig = {0};

/****** Declaración de funciones privadas ********************************************************/


/****** Definición de funciones privadas *********************************************************/


/****** Definición de funciones públicas *********************************************************/

/*-------------------------------------------------------------------------------------------------
 * @brief  Tarea que actualiza el estado de los leds
 * @param	Ninguno
 */
void TareaTest_1( void *pvParameters )
{
	/* Variables locales ------------------------------------------------------*/
	uint32_t tiempo1, tiempo2;

	/* Imprimir la tarea inicializada -----------------------------------------*/
	char *pcTaskName = (char *) pcTaskGetName( NULL );
	uoEscribirTxt3 ( "MSJ ", pcTaskName, " esta ejecutandose.\n\r" );

	/* Configuro y enciendo leds ----------------------------------------------*/
	configASSERT (	ERROR_LED !=
						(LedRojoEnPlaca = LedsRTOS_InicializarLed ( U_LED_ROJO_EP )) );
	configASSERT ( LedsRTOS_ModoLed ( LedRojoEnPlaca, SUSPENSION ) );
	configASSERT ( ERROR_LED !=
						(LedAzulEnPlaca  = LedsRTOS_InicializarLed ( U_LED_AZUL_EP )) );
	configASSERT ( LedsRTOS_ModoLed ( LedAzulEnPlaca, BALIZA ) );
	// configASSERT ( ERROR_LED !=
	// (LedVerdeEnPlaca = TareaLeds_InicializarLed ( U_LED_VERDE_EP )) );
	configASSERT ( LedsRTOS_EncenderLed (LedRojoEnPlaca)  );
	configASSERT ( LedsRTOS_EncenderLed (LedAzulEnPlaca)  );
	//configASSERT ( TareaLeds_EncenderLed (LedVerdeEnPlaca) );

	/* Enciendo GENERADORES ---------------------------------------------------*/
	Frecuencia = 9950;
	if ( GenRTOS_Obtener ( GENERADOR_1, &GenConfig, portMAX_DELAY )) {
		//uoEscribirTxtUint("Probando ", GenConfig.Maximo);
		GenConfig.Frecuencia = Frecuencia;
		GenConfig.Tipo = SENOIDAL;
		//GenConfig.Tipo = TRIANGULAR;
		//GenConfig.Tipo = CUADRADA;
		GenConfig.Simetria = 0.5;
		if (!GenRTOS_Configurar ( GENERADOR_1, &GenConfig, portMAX_DELAY ))
			uoHuboErrorTxt("No pudimos configurar GEN.");
		GenConfig.Tipo = CUADRADA;
		//GenConfig.Tipo = TRIANGULAR;
		GenConfig.Tipo = SENOIDAL;
		GenConfig.Fase = 90;
		GenConfig.Simetria = 0.5;
		GenConfig.Maximo = 2.5;
		GenConfig.Minimo = 0.5;
		//GenConfig.Frecuencia = Frecuencia;
		if (!GenRTOS_Configurar ( GENERADOR_2, &GenConfig, portMAX_DELAY ))
			uoHuboErrorTxt("No pudimos configurar GEN.");
		GenRTOS_Encender ( GENERADORES_TODOS, portMAX_DELAY );
	}

	/* Esta sería una forma de utilizar las funciones de uHALdac.h:
	uHALdacInicializar ( UHAL_DAC_TODOS );
	uHALdacdmaComenzar ( UHAL_DAC_1, senial1, 45 );
	uHALdacdmaComenzar ( UHAL_DAC_2, senial1, 45 );
	uHALdacEstablecerValor ( UHAL_DAC_2, 2000 );
	*/

	/* Pruebo ouMili y uoMicro ------------------------------------------------*/
	tiempo2 = uoMicrosegundos();
  	tiempo1 = uoMilisegundos();
  	while ( (uoMilisegundos()-tiempo1) < 5) {
  	  		// NADA!!!
  	}
  	uoEscribirTxtUintTxt ("Microsegundos tras 5 milisegundos: ", uoMicrosegundos()-tiempo2, "\n\r");
  	tiempo2 = uoMicrosegundos();
  	tiempo1 = uoMilisegundos();
  	while ( (uoMicrosegundos()-tiempo2) < 2000) {
  	  		// NADA!!!
  	}
  	uoEscribirTxtUintTxt ("Milisegundos tras 2000 microsegundos: ", uoMilisegundos()-tiempo1, "\n\r");

	/* Ciclo infinito... ------------------------------------------------------*/
	for( ;; )
	{
		LedsRTOS_ModoLed( LedAzulEnPlaca, TITILANTE );
		//if (!CaptuRTOS_Comenzar(ESPERA_CAPTU)) apli_alerta ("No pudimos comenzar CAPTURADORA.");
		vTaskDelay( 4 * DELTAT_TEST );

		/*LedsRTOS_ModoLed( LedAzulEnPlaca, TITILANTE_LENTO );
		if (!CaptuRTOS_Comenzar(ESPERA_CAPTU)) apli_alerta ("No pudimos comenzar CAPTURADORA.");
		vTaskDelay( 1UL );
		CaptuRTOS_Parar(ESPERA_CAPTU);
		vTaskDelay( DELTAT_TEST );*/

		LedsRTOS_ModoLed( LedAzulEnPlaca, PLENO );
		/*CaptuRTOS_Obtener( &CaptuConfig, ESPERA_CAPTU );
		CaptuConfig.OrigenDisparo = ENTRADA_2;
		CaptuRTOS_Configurar( &CaptuConfig, ESPERA_CAPTU );
		if (!CaptuRTOS_Comenzar(ESPERA_CAPTU)) apli_alerta ("No pudimos comenzar CAPTURADORA.");
		vTaskDelay( DELTAT_TEST );*/

		/*LedsRTOS_ModoLed( LedAzulEnPlaca, BALIZA );
		CaptuRTOS_EntradaObtener( ENTRADA_2, &EntraConfig, ESPERA_CAPTU);
		EntraConfig.NivelDisparo = 1.65;
		EntraConfig.FlancoDisparo = BAJADA;
		CaptuRTOS_EntradaConfigurar( ENTRADA_2, &EntraConfig, ESPERA_CAPTU);
		if (!CaptuRTOS_Comenzar(ESPERA_CAPTU)) apli_alerta ("No pudimos comenzar CAPTURADORA.");
		*/

		//apli_mensaje(".Nuevo ciclo de led azul en placa.\n\r", UN_SEGUNDO );

		vTaskDelay( 2 * DELTAT_TEST );
	}
}

/****************************************************************** FIN DE ARCHIVO ***************/
