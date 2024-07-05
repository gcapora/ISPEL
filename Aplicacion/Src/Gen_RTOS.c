/**************************************************************************************************
* @file		....c
* @author	Guillermo Caporaletti
* @brief
* @date
*
**************************************************************************************************/


/****** Librerías (includes) *********************************************************************/

#include "Gen_RTOS.h"

/****** Definiciones privadas (macros) ***********************************************************/

#define RETARDO_GENERADOR		pdMS_TO_TICKS( 10UL )

/****** Definiciones privadas de tipos (private typedef) *****************************************/


/****** Definición de datos privados *************************************************************/

SemaphoreHandle_t GenMutexAdmin;			// Semáforo que administra el acceso a las funciones.
gen_conf_s			GenConfigRTOS = {0};	// Configuración DAC

/****** Definición de datos públicos *************************************************************/

//const char * Entrada_1 = "ENTRADA 1";
//const char * Entrada_2 = "ENTRADA 2";

/****** Declaración de funciones privadas ********************************************************/

void escribir_salida(gen_id_e);

/****** Definición de funciones públicas *********************************************************/

/*=================================================================================================
* @brief		Inicializa recursos de GenRTOS
* @param		Ninguno
* @retval	true, si no hubo problemas
*/
bool GenRTOS_Inicializar (void)
{
	bool_t RET = true;

	// Inicializamos interfase uCapturadora
	if ( !uGeneradorInicializar(GENERADORES_TODOS) ) RET = false;

	// Creamos un semáforo Mutex para el acceso concurrente a leds
	GenMutexAdmin = xSemaphoreCreateMutex();
	configASSERT ( NULL != GenMutexAdmin );

	// Terminada la inicialización...
	return RET;
}

/*=================================================================================================
* @brief		Pedido de Encendido de Generador
* @param		ticks que puede esperar.
* @retval	true, si logró comenzar.
*/
bool GenRTOS_Encender ( gen_id_e ID, TickType_t ESPERA )
{
	bool_t RET = false;
	if(pdTRUE == xSemaphoreTake( GenMutexAdmin, ESPERA )) {
		if(uGeneradorEncender(ID)) {
			if(ID==GENERADOR_1) {
				apli_mensaje ("GEN S1 encendido.", portMAX_DELAY );
			} else if(ID==GENERADOR_2){
				apli_mensaje ("GEN S2 encendido.", portMAX_DELAY );
			} else if(ID==GENERADORES_TODOS){
				apli_mensaje ("GEN TODOS encendidos.", portMAX_DELAY );
			}
			RET = true;
		}
		xSemaphoreGive( GenMutexAdmin );
	}
	return RET;
}

bool GenRTOS_Apagar ( gen_id_e ID, TickType_t ESPERA )
{
	bool_t RET = false;
	if(pdTRUE == xSemaphoreTake( GenMutexAdmin, ESPERA )) {
		if(uGeneradorApagar(ID)) {
			if(ID==GENERADOR_1) {
				apli_mensaje ("GEN S1 apagado.", portMAX_DELAY );
			} else if(ID==GENERADOR_2){
				apli_mensaje ("GEN S2 apagado.", portMAX_DELAY );
			} else if(ID==GENERADORES_TODOS){
				apli_mensaje ("GEN TODOS apagados.", portMAX_DELAY );
			}
			RET = true;
		}
		xSemaphoreGive( GenMutexAdmin );
	}
	return RET;
}

bool GenRTOS_Configurar ( gen_id_e ID, gen_conf_s * CONFIG, TickType_t ESPERA )
{
	bool_t RET = false;
	//char TEST[5] = {0};
	if(pdTRUE == xSemaphoreTake( GenMutexAdmin, ESPERA )) {
		//TEST[0] = CONFIG->Acople + '0';
		//apli_mensaje(TEST, ESPERA);
		if(CONFIG->Frecuencia > FREC_MAX_SENIAL_GEN)
			CONFIG->Frecuencia = FREC_MAX_SENIAL_GEN;
		if(uGeneradorConfigurar(ID,CONFIG)) {
			escribir_salida(ID);
			/*uoEscribirTxt ("MSJ Configuramos Generador (tarea). \n\r");
			uoEscribirTxtUint (	"MSJ Frecuencia de muestreo = ",
										uHALdacdmaLeerFrecuenciaMuestreo((dac_id_e )ID)/1000	);
			uoEscribirTxt (" kHz\n\r");*/
			RET = true;
		}
		xSemaphoreGive( GenMutexAdmin );
	}
	return RET;
}

bool GenRTOS_Obtener ( gen_id_e ID, gen_conf_s * CONFIG, TickType_t ESPERA )
{
	bool_t RET = false;
	if(pdTRUE == xSemaphoreTake( GenMutexAdmin, ESPERA )) {
		if(uGeneradorLeerConfiguracion(ID,CONFIG)) {
			RET = true;
		}
		xSemaphoreGive( GenMutexAdmin );
	}
	return RET;
}

bool GenRTOS_EscribirConfiguraciones ( TickType_t ESPERA )
{
	bool_t		RET = false;
	if(pdTRUE == xSemaphoreTake( GenMutexAdmin, ESPERA )) {
		RET = true;
		tomar_escritura(ESPERA);
		escribir_salida(GENERADOR_1);
		escribir_salida(GENERADOR_2);
		devolver_escritura();
		xSemaphoreGive( GenMutexAdmin );
	}
	return RET;
}

/****** Definición de funciones privadas *********************************************************/

void escribir_salida(gen_id_e ID)
{
	gen_conf_s	CONFIG = {0};
	if(GENERADORES_TODOS==ID) {
		escribir_salida(GENERADOR_1);
		escribir_salida(GENERADOR_2);
	} else if(uGeneradorLeerConfiguracion(ID,&CONFIG)) {
		if(ID==GENERADOR_1){
			uoEscribirTxt("MSJ GEN S1 CONFIGURADO");
		} else if(ID==GENERADOR_2){
			uoEscribirTxt("MSJ GEN S2 CONFIGURADO");
		} else{
			uoEscribirTxt("MSJ GEN desconocido");
		}
		if(CONFIG.Tipo==CUADRADA) {
			uoEscribirTxt(" TIPO=CUADRADA");
		} else if (CONFIG.Tipo==TRIANGULAR) {
			uoEscribirTxt(" TIPO=TRIANGULAR");
		} else if (CONFIG.Tipo==SENOIDAL) {
			uoEscribirTxt(" TIPO=SENOIDAL");
		} else {
			uoEscribirTxt(" TIPO=desconocido");
		}
		uoEscribirTxtUint(" FM=",uHALdacdmaLeerFrecuenciaMuestreo(ID));
		//uoEscribirTxtReal(" LARGO=",CONFIG.Largo, 1);
		uoEscribirTxtReal(" FREC=",CONFIG.Frecuencia, 2);
		uoEscribirTxtUint(" FASE=",(uint32_t)CONFIG.Fase);
		uoEscribirTxtReal(" VMAX=",CONFIG.Maximo, 3);
		uoEscribirTxtReal(" VMIN=",CONFIG.Minimo, 3);
		uoEscribirTxtReal(" SIM=",CONFIG.Simetria, 3);
		if(CONFIG.Acople==DC) {
			uoEscribirTxt(" ACOPLE=DC");
		} else if (CONFIG.Acople==CERO) {
			uoEscribirTxt(" ACOPLE=CERO");
		} else {
			uoEscribirTxt(" ACOPLE=desconocido");
		}
		uoEscribirTxt("\n");
	}
}

/****************************************************************** FIN DE ARCHIVO ***************/
