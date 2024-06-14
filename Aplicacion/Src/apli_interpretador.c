/**************************************************************************************************
* @file	: apli.interpretador.c
* @brief	: Carga mensajes de entrada e interpreta.
*
**************************************************************************************************/

/* Includes **************************************************************************************/

#include "apli_interpretador.h"

/* Private macro *********************************************************************************/

#define MENSAJES_MAX_EN_COLA	5

/* Definiciones privadas de tipos de datos (private typedef) *************************************/

typedef char mensaje_t[MENSAJE_LARGO_MAX+1];

/* Definiciones privadas de constantes ***********************************************************/


/* Private variables *****************************************************************************/

//static char			BuferLectura[MENSAJE_LARGO_MAX+1] = {0};
static mensaje_t	BuferLectura = {0};  // Debo asegurar un '\0' al final
static uint32_t	LargoMensaje = 0;
QueueHandle_t 		xColaMensajesParaInterpretar;

/* Variables importadas **************************************************************************/


/* Private function prototypes *******************************************************************/

bool caracter_validado( char * );
bool comparar_texto ( char * , char * );
void cmd_capturadora( char * );
void cmd_generador( char * );
char* subcomando( char * , char * );


/* Public function *******************************************************************************/

bool ai_inicializar( void )
{
	xColaMensajesParaInterpretar = xQueueCreate( MENSAJES_MAX_EN_COLA, sizeof(mensaje_t) );
	if (xColaMensajesParaInterpretar == NULL) {
	   // La cola no se pudo crear
		return false;
	} else {
	   // La cola se creó exitosamente
		return true;
	}
}

void ai_cargar_caracter( char LECTURA )
{

	// Filtramos el caracter LF si existe.
	// (el filtro puede ser más sofisticado)
	if( caracter_validado(&LECTURA) ) {

		// Cargamos un caracter
		BuferLectura[LargoMensaje] = LECTURA;
		LargoMensaje++;

		// Verificamos si llegamos a LF o completamos largo de mensaje
		if( '\n'==LECTURA || (LargoMensaje==MENSAJE_LARGO_MAX) ) {

			// Cargamos mensaje y reseteamos LargoMensaje
			// (debo asegurar un '\0' al final)
			BuferLectura[LargoMensaje] = '\0';
			ai_cargar_mensaje( BuferLectura, LargoMensaje );
			LargoMensaje=0;
		}
	}
}

void ai_cargar_mensaje( char * BUFER, uint32_t LARGO )
{
	mensaje_t * PMENSAJE = (mensaje_t *) &BUFER[0];
	if (xQueueSend(xColaMensajesParaInterpretar, PMENSAJE, 100) != pdPASS) {
	   // La tarea no pudo enviar el valor a la cola
		apli_alerta("Se perdio un MENSAJE leido por falta de espacio en cola.");
	}
	//uoEscribirTxt(BUFER);
}

bool ai_procesar_mensajes( void )
{
	mensaje_t	MsjParaProcesar = {0};
	bool_t	 	AlgoProcesado = false;
	char *		SUBCMD = "";

	while (xQueueReceive(xColaMensajesParaInterpretar, &MsjParaProcesar, 10) == pdPASS) {
		AlgoProcesado = true;

		if ( comparar_texto(MsjParaProcesar, CMD_HOLA) ){
			SUBCMD = subcomando( MsjParaProcesar, CMD_HOLA );
			tomar_escritura   ( portMAX_DELAY );
			uoEscribirTxt2    ( "HOLA, aca ISPEL: ", SUBCMD );
			devolver_escritura();

		} else if ( comparar_texto(MsjParaProcesar, CMD_CAPTURADORA) ){
			cmd_capturadora( MsjParaProcesar );

		} else if ( comparar_texto(MsjParaProcesar, CMD_GENERADOR) ){
			cmd_generador( MsjParaProcesar );

		} else {
			uoEscribirTxt( (char *) MsjParaProcesar);
		}

	}
	return AlgoProcesado;
}

bool caracter_validado( char * PCARACTER )
{
	// Variables globales
	static bool EsPrimerCaracter = true;
	static bool EspacioPrevio = false;
	bool RET = false;

	// Correcciones de caracteres ---------------------------

	// Paso a mayúscula:
	if ( 97 <= *PCARACTER &&
		  122>= *PCARACTER ) {
		*PCARACTER = *PCARACTER-32;
	}

	// TAB, ',', ':' ---> espacio
	if ('\t'==*PCARACTER || ','==*PCARACTER || ':'==*PCARACTER) {
		*PCARACTER=' ';
	}

	// Filtrado ---------------------------------------------

	// ¿Es un caracter a filtrar?
	if ( '\r'==*PCARACTER ||
		  '\0'==*PCARACTER ) {
		// Caracteres que destaco como no válidos
		RET = false;

	// ¿Es el fin de mensaje?
	} else if ( '\n'==*PCARACTER ) {
		RET = true;
		EsPrimerCaracter = true;

	// ¿Estamos leyendo el inicio de un mensaje?
	} else if (EsPrimerCaracter) {
		if ( 65<=*PCARACTER && 90>=*PCARACTER ) {
			RET = true;
			EsPrimerCaracter = false;
			EspacioPrevio = false;
		} else {
			RET = false;
		}

	// ¿Es caracter válido dentro de mensaje? (casos absolutos)
	} else if ( (65<=*PCARACTER && 90>=*PCARACTER) || // letras mayúsculas
				   (43<=*PCARACTER && 57>=*PCARACTER) || // números y puntuación
					('='==*PCARACTER) ||
					(';'==*PCARACTER) ||
					('_'==*PCARACTER)    ) {
		RET = true;
		EspacioPrevio = false;

	// ¿Es doble espacio? (caso condicional)
	} else if (' '==*PCARACTER) {
		if (EspacioPrevio) {
			RET = false;
		} else {
			RET = true;
			EspacioPrevio = true;
		}

	// Es algún otro caracter no admitido.
	} else {
		RET = false;
	}

	// Fin!
	return RET;
}

bool comparar_texto ( char * TEXTO1, char * COMPARADO)
{
	if ( strncmp (TEXTO1, COMPARADO, strlen(COMPARADO)) == 0 &&
		( TEXTO1[strlen(COMPARADO)]==' ' || TEXTO1[strlen(COMPARADO)]=='\n' )) {
		return true;
	}
	return false;
}

void cmd_capturadora( char * COMANDO )
{
	// Variables locales
	char * SUBCMD = subcomando( COMANDO, CMD_CAPTURADORA );

	// Analizo subcomando

	// INICIAR
	if (comparar_texto(SUBCMD,CMD_INICIAR)) {
		CaptuRTOS_Comenzar(portMAX_DELAY);

	// PARAR
	} else if (comparar_texto(SUBCMD,CMD_PARAR)) {
		CaptuRTOS_Parar(portMAX_DELAY);

	// CONFIGURAR
	} else if (comparar_texto(SUBCMD,CMD_CONFIGURAR)) {
		apli_mensaje( "Aun no puedo configurar Capturadora...\n", portMAX_DELAY );

	// CONFIGURAR
	} else if (comparar_texto(SUBCMD,CMD_OBTENER)) {
		apli_mensaje( "Debo enviarte configuraciones de capturadora...\n", portMAX_DELAY );

	// ENTRADA
	} else if (comparar_texto(SUBCMD,CMD_ENTRADA)) {
		apli_mensaje( "Debo ver ENTRADA...\n", portMAX_DELAY );

	// Subcomando NO RECONOCIDO
	} else {
		apli_mensaje( "Comando de Capturadora no reconocido.\n", portMAX_DELAY );
	}
}

void cmd_generador( char * COMANDO )
{
	apli_mensaje( "Generador!\n", portMAX_DELAY );
}

char* subcomando( char * MSJ, char * CMD )
{
	char * SUBCMD;
	// ¿Hay subcomando?
	if (strlen(MSJ) > strlen(CMD)+1) {
		// Hay sub comando (tomo lo que sigue al CMD ppal):
		SUBCMD = &MSJ[strlen(CMD)+1];
	} else {
		// Subcomando: no hay.
		SUBCMD = "\n";
	}
	return SUBCMD;
}

