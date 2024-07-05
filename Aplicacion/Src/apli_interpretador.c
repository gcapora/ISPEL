/**************************************************************************************************
* @file	: apli.interpretador.c
* @brief	: Carga mensajes de entrada e interpreta.
*
**************************************************************************************************/

/* Includes **************************************************************************************/

#include <string.h>
#include <stdlib.h>
#include "apli_interpretador.h"

/* Private macro *********************************************************************************/

#define MENSAJES_MAX_EN_COLA	5

/* Definiciones privadas de tipos de datos (private typedef) *************************************/

typedef char mensaje_t[MENSAJE_LARGO_MAX+1];
typedef char atributo_valor_t[VALOR_LARGO_MAX+1];
typedef enum {
	TIPO_SENIAL,
	FREC_SENIAL,
	FASE_SENIAL,
	VMAX_SENIAL,
	VMIN_SENIAL,
	SIM_SENIAL,
	ACOPLE_SENIAL,
	NUM_ATRIBUTOS,
	ATRIBUTO_NO_IDENTIFICADO
} atributo_e;

/* Definiciones privadas de constantes ***********************************************************/


/* Private variables *****************************************************************************/

static mensaje_t	BuferLectura = {0};  // Debo asegurar un '\0' al final
static uint32_t	LargoMensaje = 0;
static char *		ATRIBUTO_ID_TXT [NUM_ATRIBUTOS] = {0};
//static bool			AtributoLeido [NUM_ATRIBUTOS] = {0};
QueueHandle_t 		xColaMensajesParaInterpretar;

/* Variables importadas **************************************************************************/


/* Private function prototypes *******************************************************************/

bool		caracter_validado( char * );
bool		comparar_texto( char *, char * );
void 		cmd_capturadora( char * );
void 		cmd_generador( char * );
char* 	subcomando( char *, char * );
char*		obtener_atributo( char *, atributo_e *, atributo_valor_t );
bool		cargar_atributo_gen( gen_conf_s *, atributo_e, atributo_valor_t );
gen_id_e	identificar_generador( char * );
uint32_t	indice_char_buscado(char *, char );

/* Public function *******************************************************************************/

bool ai_inicializar( void )
{
	bool RETORNO = false;

	// Cola de mensajes
	xColaMensajesParaInterpretar = xQueueCreate( MENSAJES_MAX_EN_COLA, sizeof(mensaje_t) );
	if (xColaMensajesParaInterpretar != NULL) RETORNO = true;

	// Textos de atributos
	ATRIBUTO_ID_TXT [TIPO_SENIAL]   = "TIPO";
	ATRIBUTO_ID_TXT [FREC_SENIAL]   = "FREC";
	ATRIBUTO_ID_TXT [FASE_SENIAL]   = "FASE";
	ATRIBUTO_ID_TXT [VMAX_SENIAL]   = "VMAX";
	ATRIBUTO_ID_TXT [VMIN_SENIAL]   = "VMIN";
	ATRIBUTO_ID_TXT [SIM_SENIAL]    = "SIM";
	ATRIBUTO_ID_TXT [ACOPLE_SENIAL] = "ACOPLE";

	// Fin
	return RETORNO;
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

/* Funciones privadas *****************************************************************************/

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
		( TEXTO1[strlen(COMPARADO)]==' ' || TEXTO1[strlen(COMPARADO)]=='\n' || TEXTO1[strlen(COMPARADO)]=='\0')) {
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
		apli_mensaje( "Aun no puedo configurar Capturadora...", portMAX_DELAY );

	// CONFIGURAR
	} else if (comparar_texto(SUBCMD,CMD_OBTENER)) {
		apli_mensaje( "Debo enviarte configuraciones de capturadora...", portMAX_DELAY );

	// ENTRADA
	} else if (comparar_texto(SUBCMD,CMD_ENTRADA)) {
		apli_mensaje( "Debo ver ENTRADA...", portMAX_DELAY );

	// Subcomando NO RECONOCIDO
	} else {
		apli_mensaje( "Comando de Capturadora no reconocido.", portMAX_DELAY );
	}
}

void cmd_generador( char * COMANDO )
{
	// Variables locales
	char *	SUBCMD = subcomando( COMANDO, CMD_GENERADOR );
	char *	TEXTO = NULL;
	bool_t	DeboConfigurar = false;
	gen_id_e	GeneradorId = GENERADOR_NO_IDENTIFICADO;
	gen_conf_s	GeneradorConfig = {0};
	atributo_valor_t	AtributoValorTxt = {0};
	atributo_e			AtributoId = ATRIBUTO_NO_IDENTIFICADO;


	// Analizo subcomando

	// ENCENDER
	if (comparar_texto(SUBCMD,CMD_ENCENDER)) {
		TEXTO = subcomando( SUBCMD, CMD_ENCENDER );
		GeneradorId = identificar_generador( TEXTO );
		if(GeneradorId<=CANTIDAD_GENERADORES)
			GenRTOS_Encender( GeneradorId, portMAX_DELAY );

	// APAGAR
	} else if (comparar_texto(SUBCMD,CMD_APAGAR)) {
		TEXTO = subcomando( SUBCMD, CMD_APAGAR );
		GeneradorId = identificar_generador( TEXTO );
		if(GeneradorId<=CANTIDAD_GENERADORES)
			GenRTOS_Apagar( GeneradorId, portMAX_DELAY );

	// CONFIGURAR
	} else if (comparar_texto(SUBCMD,CMD_CONFIGURAR)) {
		// Identifico Generador y actualizo TEXTO
		TEXTO = subcomando( SUBCMD, CMD_CONFIGURAR );
		GeneradorId = identificar_generador( TEXTO );
		if (GeneradorId==GENERADOR_1) {
			TEXTO = subcomando( TEXTO, CMD_SALIDA_1 );
		} else if (GeneradorId==GENERADOR_2) {
			TEXTO = subcomando( TEXTO, CMD_SALIDA_2 );
		} else if (GeneradorId==GENERADORES_TODOS) {
			TEXTO = subcomando( TEXTO, CMD_SALIDA_X );
		}
		// Obtengo parámetros actuales
		if (GENERADOR_2==GeneradorId) {
			GenRTOS_Obtener ( GENERADOR_2, &GeneradorConfig, portMAX_DELAY );
		} else {
			GenRTOS_Obtener ( GENERADOR_1, &GeneradorConfig, portMAX_DELAY );
		}
		// Busco atributos
		if(GeneradorId<=CANTIDAD_GENERADORES) {
			do {
				TEXTO = obtener_atributo( TEXTO, &AtributoId, AtributoValorTxt );
				if(AtributoId<NUM_ATRIBUTOS) {
					if(true==cargar_atributo_gen(&GeneradorConfig, AtributoId, AtributoValorTxt))
						DeboConfigurar = true;
					//apli_mensaje( ATRIBUTO_ID_TXT[AtributoId], portMAX_DELAY );
					//apli_mensaje( AtributoValorTxt, portMAX_DELAY );
				}
			} while(AtributoId<NUM_ATRIBUTOS);
			if(true==DeboConfigurar) {
				apli_mensaje( "Intentamos configurar.", portMAX_DELAY );
				GenRTOS_Configurar( GeneradorId,&GeneradorConfig, portMAX_DELAY );
			}

		}

	// OBTENER
	} else if (comparar_texto(SUBCMD,CMD_OBTENER)) {
		GenRTOS_EscribirConfiguraciones(portMAX_DELAY);

	// Subcomando NO RECONOCIDO
	} else {
		apli_mensaje( "Subomando GEN no reconocido.", portMAX_DELAY );
	}
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

gen_id_e identificar_generador (char * TEXTO)
{
	gen_id_e Generador = GENERADOR_NO_IDENTIFICADO;
	if (comparar_texto(TEXTO,CMD_SALIDA_1)) {
		Generador = GENERADOR_1;
		//TEXTO = subcomando( TEXTO, CMD_SALIDA_1 );
	} else if (comparar_texto(TEXTO,CMD_SALIDA_2)) {
		Generador = GENERADOR_2;
		//TEXTO = subcomando( TEXTO, CMD_SALIDA_2 );
	} else if (comparar_texto(TEXTO,CMD_SALIDA_X)) {
		Generador = GENERADORES_TODOS;
		//TEXTO = subcomando( TEXTO, CMD_SALIDA_X );
	}
	if(Generador==GENERADOR_NO_IDENTIFICADO)
		apli_mensaje( "Generador no identificado.", portMAX_DELAY );
	return Generador;
}

char * obtener_atributo( char * TEXTO, atributo_e * P_ATRIB_HALLADO, atributo_valor_t ATRIB_VALOR )
{
	// Variables locales ---------------------------------------------
	char *	AtributoAnalizado = NULL;
	char *	AtributoActual = NULL;
	char *	AtributoProximo = NULL;
	uint16_t LargoAtributoAnalizado = 0;
	uint16_t	e=0;
	uint16_t f=0;
	// Comprobación atributo a atributo ------------------------------
	*P_ATRIB_HALLADO = ATRIBUTO_NO_IDENTIFICADO;
	for (e=0; e<NUM_ATRIBUTOS; e++) {
		AtributoAnalizado      = ATRIBUTO_ID_TXT[e];
		LargoAtributoAnalizado = strlen(AtributoAnalizado);
		if( strncmp(TEXTO,AtributoAnalizado,LargoAtributoAnalizado)==0 ) {
			if( TEXTO[LargoAtributoAnalizado] == '=' ) {
				// Si coincide texto y tengo un '=' => tengo un atributo
				*P_ATRIB_HALLADO = e;
				AtributoActual = &TEXTO[LargoAtributoAnalizado+1];
				AtributoProximo = strchr( AtributoActual, ' ' );
				if (NULL==AtributoProximo) {
					// No hay más atributos a analizar
					for (f=0; f<VALOR_LARGO_MAX && f<strlen(AtributoActual); f++ ){
						if (AtributoActual[f] == '\n') {
							ATRIB_VALOR[f] = '\0';
						} else {
							ATRIB_VALOR[f] = AtributoActual [f];
						}
					}
					ATRIB_VALOR[f] = '\0';
				} else {
					// Hay otro atributo separado por un espacio
					for (f=0; f<VALOR_LARGO_MAX && (f+AtributoActual<AtributoProximo); f++ ){
						ATRIB_VALOR[f] = AtributoActual [f];
					}
					ATRIB_VALOR[f] = '\0';
					AtributoProximo++;
				}
			}
		}
	}
	return AtributoProximo;
}

bool cargar_atributo_gen(gen_conf_s * CONFIG, atributo_e ATRID, atributo_valor_t ATRVLR)
{
	// Variables locales:
	bool		RET = false;
	double	NUM = 0;
	char*		RETCON = NULL;

	// Evalúo:
	switch (ATRID) {
	case TIPO_SENIAL:
		if(0==strcmp("SENO", ATRVLR)    ||
			0==strcmp("SENOIDAL", ATRVLR)  ) {
			CONFIG->Tipo = SENOIDAL;
			RET = true;
		} else if(0==strcmp("CUADRADA", ATRVLR)) {
			CONFIG->Tipo = CUADRADA;
			RET = true;
		} else if(0==strcmp("TRIANGULAR", ATRVLR)) {
			CONFIG->Tipo = TRIANGULAR;
			RET = true;
		}
		break;

	case ACOPLE_SENIAL:
		if(0==strcmp("DC", ATRVLR)) {
			CONFIG->Acople = DC;
			RET = true;
		} else if(0==strcmp("CERO", ATRVLR)) {
			CONFIG->Acople = CERO;
			RET = true;
		}
		break;

	case FREC_SENIAL:
		NUM = strtod(ATRVLR, &RETCON);
		if(NUM>0) {
			CONFIG->Frecuencia = NUM;
			RET = true;
		}
		break;

	case FASE_SENIAL:
		NUM = strtod(ATRVLR, &RETCON);
		if(*RETCON=='\0') {
			CONFIG->Fase = NUM;
			RET = true;
		}
		break;

	case VMAX_SENIAL:
		NUM = strtod(ATRVLR, &RETCON);
		if(*RETCON=='\0') {
			CONFIG->Maximo = NUM;
			RET = true;
		}
		break;

	case VMIN_SENIAL:
		NUM = strtod(ATRVLR, &RETCON);
		if(*RETCON=='\0') {
			CONFIG->Minimo = NUM;
			RET = true;
		}
		break;

	case SIM_SENIAL:
		NUM = strtod(ATRVLR, &RETCON);
		if(*RETCON=='\0') {
			CONFIG->Simetria = NUM;
			RET = true;
		}
		break;

	default:
		RET = false;
	}

	return RET;
}

