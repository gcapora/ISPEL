/**************************************************************************************************
* @file		Test_RTOS.c
* @author	Guillermo Caporaletti
* @brief	Tarea para testear aplicación.
* @date		2024
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
	uoEscribirTxt3 ( "MSJ ", pcTaskName, " esta ejecutandose.\r\n" );

	/* Configuro y enciendo leds ----------------------------------------------*/
	configASSERT ( ERROR_LED != (LedAzulEnPlaca  = LedsRTOS_InicializarLed ( U_LED_AZUL_EP )) );
	configASSERT ( LedsRTOS_ModoLed ( LedAzulEnPlaca, BALIZA ) );
	configASSERT ( LedsRTOS_EncenderLed (LedAzulEnPlaca)  );

	/* Configuración inicial de generadores y encendido */
	config_0_generadores();
	//encender_generadores();
	//testeo_temporizadores();

	/* Ciclo infinito... ------------------------------------------------------*/
	for( ;; )
	{
		LedsRTOS_ModoLed( LedAzulEnPlaca, PLENO );
		vTaskDelay( 2 * DELTAT_TEST );

		LedsRTOS_ModoLed( LedAzulEnPlaca, BALIZA );
		vTaskDelay( 10 * DELTAT_TEST );

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

void Test_Generador ( TickType_t ESPERA )
{
	gen_conf_s	GenConfigTest = {0};
	if(pdTRUE == xSemaphoreTake( TestMutexAdmin, ESPERA )) {

		// Mensaje inicial
		apli_mensaje("Comenzando TEST de Generador.", UN_SEGUNDO);
		apli_mensaje("Precondicion: Las salidas S1 y S2 deben estar conectadas a un osciloscopio.", UN_SEGUNDO);

		// Configuro generadores en señal CUADRADA
		if ( GenRTOS_Obtener ( GENERADOR_1, &GenConfigTest, ESPERA )) {
			GenConfigTest.Frecuencia = 1000;
			GenConfigTest.Tipo = CUADRADA;
			GenConfigTest.Fase = 0;
			GenConfigTest.Simetria = 0.5;
			GenConfigTest.Maximo = 4.0;
			GenConfigTest.Minimo = 0.0;
			GenConfigTest.Acople = DC;
			GenRTOS_Configurar ( GENERADOR_1, &GenConfigTest, portMAX_DELAY );
			GenRTOS_Configurar ( GENERADOR_2, &GenConfigTest, portMAX_DELAY );
			encender_generadores();
			apli_mensaje("SX encendidas con senal CUADRADA 0V-4V 1kHz.", 3 * UN_SEGUNDO);
			vTaskDelay( 30 * UN_SEGUNDO );
		} else {
			apli_mensaje("No pudimos ejecutar testeo de Generador (senal cuadrada).", 3 * UN_SEGUNDO);
			return;
		}

		// Congiguramos senial 10 kHz CUADRADA y TRIANGULAR
		GenConfigTest.Frecuencia = 10000;
		GenConfigTest.Tipo = CUADRADA;
		GenConfigTest.Fase = 0;
		GenConfigTest.Simetria = 0.5;
		GenConfigTest.Maximo = 4.0;
		GenConfigTest.Minimo = -4.0;
		GenConfigTest.Acople = DC;
		GenRTOS_Configurar ( GENERADOR_1, &GenConfigTest, portMAX_DELAY );
		GenConfigTest.Tipo = TRIANGULAR;
		GenRTOS_Configurar ( GENERADOR_2, &GenConfigTest, portMAX_DELAY );
		apli_mensaje("SX con senales TRIANGULAR y CUADRADA 6V pico 10kHz.", 3 * UN_SEGUNDO);
		vTaskDelay( 30 * UN_SEGUNDO );

		// Congiguramos senial 9V
		GenConfigTest.Maximo = 9.0;
		GenConfigTest.Minimo = -9.0;
		GenConfigTest.Tipo = SENOIDAL;
		GenRTOS_Configurar ( GENERADOR_1, &GenConfigTest, portMAX_DELAY );
		GenRTOS_Configurar ( GENERADOR_2, &GenConfigTest, portMAX_DELAY );
		apli_mensaje("SX con senales 9V pico.", 3 * UN_SEGUNDO);
		vTaskDelay( 30 * UN_SEGUNDO );

		// Congiguramos senial 10 kHz CUADRADA y TRIANGULAR
		GenConfigTest.Frecuencia = 100000;
		GenConfigTest.Tipo = SENOIDAL;
		GenConfigTest.Fase = 0;
		GenConfigTest.Simetria = 0.5;
		GenConfigTest.Maximo = 1.25;
		GenConfigTest.Minimo = -1.25;
		GenConfigTest.Acople = DC;
		GenRTOS_Configurar ( GENERADOR_1, &GenConfigTest, portMAX_DELAY );
		GenRTOS_Configurar ( GENERADOR_2, &GenConfigTest, portMAX_DELAY );
		apli_mensaje("SX con senal SENOIDAL 100 kHz 1v pico.", 3 * UN_SEGUNDO);
		vTaskDelay( 30 * UN_SEGUNDO );

		// Fin
		GenRTOS_Apagar ( GENERADORES_TODOS, portMAX_DELAY );
		apli_mensaje("Terminamos TEST de Generador. Apagamos salids SX.", 3 * UN_SEGUNDO);
		xSemaphoreGive( TestMutexAdmin );
	} else {
		apli_mensaje("No pudimos ejecutar testeo de Generador (inicio).", 3 * UN_SEGUNDO);
	}
}


void Test_Capturadora (TickType_t ESPERA)
{
	capturadora_config_s	CaptuConfigTest = {0};
	entrada_config_s     EntradaConfigTest = {0};

	if(pdTRUE == xSemaphoreTake( TestMutexAdmin, ESPERA )) {

		// Mensaje inicial
		apli_mensaje("Comenzando TEST de Capturadora.", UN_SEGUNDO);
		apli_mensaje("Precondicion: Las entradas E1 y E2 deben estar conectadas a la salida de prueba.", UN_SEGUNDO);

		// Configuro Capturadora
		if ( !CaptuRTOS_Obtener ( &CaptuConfigTest, ESPERA )) {
			apli_mensaje("No pudimos ejecutar testeo de Capturadora (obteniendo configuracion).", 3 * UN_SEGUNDO);
			return;
		}
		CaptuConfigTest.EscalaHorizontal = 0.001;
		CaptuConfigTest.OrigenDisparo    = ENTRADA_1;
		CaptuConfigTest.ModoCaptura      = CAPTURA_UNICA;
		if ( !CaptuRTOS_Configurar ( &CaptuConfigTest, ESPERA )) {
			apli_mensaje("No pudimos ejecutar testeo de Capturadora (configurando).", 3 * UN_SEGUNDO);
			return;
		}

		// Configuro entradas
		if ( !CaptuRTOS_EntradaObtener ( ENTRADA_1, &EntradaConfigTest, ESPERA )) {
			apli_mensaje("No pudimos ejecutar testeo de Capturadora (obteniendo entrada).", 3 * UN_SEGUNDO);
			return;
		}
		EntradaConfigTest.EscalaVertical = 6;
		EntradaConfigTest.FlancoDisparo  = SUBIDA;
		EntradaConfigTest.NivelDisparo   = 2.5;
		if ( !CaptuRTOS_EntradaConfigurar ( ENTRADA_1, &EntradaConfigTest, ESPERA )) {
			apli_mensaje("No pudimos ejecutar testeo de Capturadora (configurando entrada).", 3 * UN_SEGUNDO);
			return;
		}
		if ( !CaptuRTOS_EntradaConfigurar ( ENTRADA_2, &EntradaConfigTest, ESPERA )) {
			apli_mensaje("No pudimos ejecutar testeo de Capturadora (configurando entrada).", 3 * UN_SEGUNDO);
			return;
		}

		// Enciendo entradas
		if ( !CaptuRTOS_EntradaEncender (ENTRADAS_TODAS, ESPERA) ) {
			apli_mensaje("No pudimos ejecutar testeo de Capturadora (encendiendo entradas).", 3 * UN_SEGUNDO);
			return;
		}

		// Capturo...
		if ( !CaptuRTOS_Comenzar(ESPERA) ) {
			apli_mensaje("No pudimos ejecutar testeo de Capturadora (capturando).", 3 * UN_SEGUNDO);
			return;
		}

		vTaskDelay( 10 * UN_SEGUNDO );
		CaptuRTOS_EntradaApagar (ENTRADAS_TODAS, portMAX_DELAY);
		apli_mensaje("TEST de Capturadora terminado.", 3 * UN_SEGUNDO);
		xSemaphoreGive( TestMutexAdmin );
	} else {
		apli_mensaje("No pudimos ejecutar TEST de Capturadora.", 3 * UN_SEGUNDO);
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
	uoEscribirTxtUintTxt ("// Microsegundos tras 5 milisegundos: ", uoMicrosegundos()-tiempo2, "\r\n");
	tiempo2 = uoMicrosegundos();
	tiempo1 = uoMilisegundos();
	while ( (uoMicrosegundos()-tiempo2) < 2000) {
		  	// NADA!!!
	}
	uoEscribirTxtUintTxt ("// Milisegundos tras 2000 microsegundos: ", uoMilisegundos()-tiempo1, "\r\n");
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
		GenConfig.Maximo = 4.0;
		GenConfig.Minimo = -4.0;
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
