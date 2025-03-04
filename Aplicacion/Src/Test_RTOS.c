/**************************************************************************************************
* @file		Test_RTOS.c
* @author	Guillermo Caporaletti
* @brief		Tarea para testear aplicación.
* @date		Julio de 2024
*
**************************************************************************************************/

/****** Librerías (includes) *********************************************************************/

#include <Test_RTOS.h>

/****** Definiciones privadas (macros) ***********************************************************/

#define DELTAT_TEST			pdMS_TO_TICKS( 1000UL )
#define ESPERA_CAPTU			pdMS_TO_TICKS( 5UL )

/****** Definiciones privadas de tipos (private typedef) *****************************************/

SemaphoreHandle_t 	TestMutexAdmin;

/****** Definición de datos públicos *************************************************************/

led_id_t    		LedAzulEnPlaca;

/****** Definición de datos privados *************************************************************/

static double		Frecuencia = 0;
static gen_conf_s	GenConfig = {0};

/****** Declaración de funciones privadas ********************************************************/

void testeo_temporizadores	(void);
void config_0_generadores	(void);
void encender_generadores	(void);

/****** Definición de funciones públicas *********************************************************/

bool TestRTOS_Inicializar (void)
{
	// Semáforo Mutex para acceso concurrente
	TestMutexAdmin = xSemaphoreCreateMutex();
	configASSERT ( NULL != TestMutexAdmin );
	//Chau!
	return true;
}

/*-------------------------------------------------------------------------------------------------
 * @brief
 * @param
 */
void Tarea_Test( void *pvParameters )
{
	/* Variables locales ------------------------------------------------------*/

	/* Imprimir la tarea inicializada -----------------------------------------*/
	char *pcTaskName = (char *) pcTaskGetName( NULL );
	uoEscribirTxt3 ( "MSJ ", pcTaskName, " esta ejecutandose.\n\r" );

	/* Configuro y enciendo leds ----------------------------------------------*/
	configASSERT ( ERROR_LED != (LedAzulEnPlaca  = LedsRTOS_InicializarLed ( U_LED_AZUL_EP )) );
	configASSERT ( LedsRTOS_ModoLed ( LedAzulEnPlaca, BALIZA ) );
	configASSERT ( LedsRTOS_EncenderLed (LedAzulEnPlaca)  );

	/* Configuración inicial de generadores y encendido */
	config_0_generadores();
	//encender_generadores();

	/* Esta sería una forma de utilizar las funciones de uHALdac.h:
	uHALdacInicializar ( UHAL_DAC_TODOS );
	uHALdacdmaComenzar ( UHAL_DAC_1, senial1, 45 );
	uHALdacdmaComenzar ( UHAL_DAC_2, senial1, 45 );
	uHALdacEstablecerValor ( UHAL_DAC_2, 2000 );
	*/

	testeo_temporizadores();

	/* Ciclo infinito... ------------------------------------------------------*/
	for( ;; )
	{

		LedsRTOS_ModoLed( LedAzulEnPlaca, PLENO );
		vTaskDelay( 1 * DELTAT_TEST );

		//if (!CaptuRTOS_Comenzar(ESPERA_CAPTU)) apli_alerta ("No pudimos comenzar CAPTURADORA.");
		/*LedsRTOS_ModoLed( LedAzulEnPlaca, TITILANTE_LENTO );
		if (!CaptuRTOS_Comenzar(ESPERA_CAPTU)) apli_alerta ("No pudimos comenzar CAPTURADORA.");
		vTaskDelay( 1UL );
		CaptuRTOS_Parar(ESPERA_CAPTU);
		vTaskDelay( DELTAT_TEST );*/

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

		LedsRTOS_ModoLed( LedAzulEnPlaca, BALIZA );
		vTaskDelay( 5 * DELTAT_TEST );
	}
}


void Test_Testear ( tipo_test_e TIPO_TEST, TickType_t ESPERA )
{
	if(pdTRUE == xSemaphoreTake( TestMutexAdmin, ESPERA )) {
		config_0_generadores();
		encender_generadores();
		vTaskDelay( ESPERA_CAPTU );
		CaptuRTOS_Comenzar( ESPERA );
		xSemaphoreGive( TestMutexAdmin );
	} else {
		apli_mensaje("No pudimos ejecutar testeo.", 3 * UN_SEGUNDO);
	}
}

void Test_Config_0 (TickType_t ESPERA)
{
	if(pdTRUE == xSemaphoreTake( TestMutexAdmin, ESPERA )) {
		config_0_generadores();
		xSemaphoreGive( TestMutexAdmin );
	} else {
		apli_mensaje("No pudimos ejecutar Test_Config_0.", 3 * UN_SEGUNDO);
	}
}

void Test_Captu_Inicia (TickType_t ESPERA)
{
	if(pdTRUE == xSemaphoreTake( TestMutexAdmin, ESPERA )) {
		CaptuRTOS_Comenzar( ESPERA );
		xSemaphoreGive( TestMutexAdmin );
	} else {
		apli_mensaje("No pudimos ejecutar Test_Captu_Inicia.", 3 * UN_SEGUNDO);
	}
}

void Test_Gen_Enciende (TickType_t ESPERA)
{
	if(pdTRUE == xSemaphoreTake( TestMutexAdmin, ESPERA )) {
		encender_generadores();
		xSemaphoreGive( TestMutexAdmin );
	} else {
		apli_mensaje("No pudimos ejecutar Test_Captu_Inicia.", 3 * UN_SEGUNDO);
	}
}

/****** Definición de funciones privadas *********************************************************/

void testeo_temporizadores (void)
{
	/* Pruebo ouMili y uoMicro ------------------------------------------------*/
	uint32_t tiempo2 = uoMicrosegundos();
	uint32_t tiempo1 = uoMilisegundos();
	while ( (uoMilisegundos()-tiempo1) < 5) {
	  		// NADA!!!
	}
	uoEscribirTxtUintTxt ("// Microsegundos tras 5 milisegundos: ", uoMicrosegundos()-tiempo2, "\n\r");
	tiempo2 = uoMicrosegundos();
	tiempo1 = uoMilisegundos();
	while ( (uoMicrosegundos()-tiempo2) < 2000) {
		  	// NADA!!!
	}
	uoEscribirTxtUintTxt ("// Milisegundos tras 2000 microsegundos: ", uoMilisegundos()-tiempo1, "\n\r");
}

void config_0_generadores (void)
{
	Frecuencia = 1000;
	if ( GenRTOS_Obtener ( GENERADOR_1, &GenConfig, portMAX_DELAY )) {
		GenConfig.Frecuencia = Frecuencia;
		GenConfig.Tipo = SENOIDAL;
		//GenConfig.Tipo = TRIANGULAR;
		//GenConfig.Tipo = CUADRADA;
		GenConfig.Fase = 0;
		GenConfig.Simetria = 0.5;
		GenConfig.Maximo = 5.02;
		GenConfig.Minimo = -5.02;
		GenConfig.Acople = DC;
		if (!GenRTOS_Configurar ( GENERADOR_1, &GenConfig, portMAX_DELAY ))
			uoHuboErrorTxt("No pudimos configurar GEN.");
		if (!GenRTOS_Configurar ( GENERADOR_2, &GenConfig, portMAX_DELAY ))
			uoHuboErrorTxt("No pudimos configurar GEN.");
	}
}

void encender_generadores(void)
{
	GenRTOS_Encender ( GENERADORES_TODOS, portMAX_DELAY );
}

/****************************************************************** FIN DE ARCHIVO ***************/
