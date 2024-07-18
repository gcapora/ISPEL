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

#define MENSAJES_MAX_EN_COLA	10

/* Definiciones privadas de tipos de datos (private typedef) *************************************/

typedef char mensaje_t[MENSAJE_LARGO_MAX+1];
typedef char atributo_valor_t[VALOR_LARGO_MAX+1];

/* Definiciones privadas de constantes ***********************************************************/


/* Private variables *****************************************************************************/

static mensaje_t	BuferLectura = {0};  // Debo asegurar un '\0' al final
static uint32_t	LargoMensaje = 0;
QueueHandle_t 		xColaMensajesParaInterpretar;

/* Variables públicas **************************************************************************/

char *	ATRIBUTO_ID_TXT [NUM_ATRIBUTOS] = {0};

/* Private function prototypes *******************************************************************/

bool		caracter_validado ( char* );
bool		comparar_texto    ( char*, char* );

void 		cmd_generador     ( char* );
void		cmd_salida			( char* );
void 		cmd_capturadora   ( char* );
void		cmd_entrada			( char* );
char* 	subcomando        ( char*, char* );

char*		obtener_atributo			( char*,                 atributo_e*, atributo_valor_t );
bool		cargar_atributo_gen		( gen_conf_s*,           atributo_e,  atributo_valor_t );
bool		cargar_atributo_captu	( capturadora_config_s*, atributo_e,  atributo_valor_t );
bool		cargar_atributo_entrada	( entrada_config_s*,     atributo_e,  atributo_valor_t );

gen_id_e			identificar_generador ( char* );
entrada_id_e	identificar_entrada ( char* );

/* Public function *******************************************************************************/

bool ai_inicializar( void )
{
	bool RETORNO = false;

	// Cola de mensajes
	xColaMensajesParaInterpretar = xQueueCreate( MENSAJES_MAX_EN_COLA, sizeof(mensaje_t) );
	if (xColaMensajesParaInterpretar != NULL) RETORNO = true;

	// Textos de atributos
	ATRIBUTO_ID_TXT [TIPO]				= "TIPO";
	ATRIBUTO_ID_TXT [FREC_SENIAL]		= "FREC";
	ATRIBUTO_ID_TXT [FASE_SENIAL]		= "FASE";
	ATRIBUTO_ID_TXT [VMAX_SENIAL]		= "VMAX";
	ATRIBUTO_ID_TXT [VMIN_SENIAL]		= "VMIN";
	ATRIBUTO_ID_TXT [SIM_SENIAL]		= "SIM";
	ATRIBUTO_ID_TXT [ACOPLE_SENIAL]	= "ACOPLE";
	ATRIBUTO_ID_TXT [ESCALA] 			= "ESCALA";
	ATRIBUTO_ID_TXT [ORIGEN] 			= "ORIGEN";
	ATRIBUTO_ID_TXT [PROMEDIO] 		= "PROM";
	ATRIBUTO_ID_TXT [NIVEL] 			= "NIVEL";
	ATRIBUTO_ID_TXT [FLANCO] 			= "FLANCO";

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

		} else if ( comparar_texto(MsjParaProcesar, CMD_MENSAJE) ||
						comparar_texto(MsjParaProcesar, CMD_MENSAJE2)  ){
			// No hago nada...

		} else if ( comparar_texto(MsjParaProcesar, CMD_TEST) ){
			Test_Testear ( TEST_GENERAL, UN_SEGUNDO );

		} else if ( comparar_texto(MsjParaProcesar, CMD_ESPERAR) ){
			vTaskDelay( 5 * UN_SEGUNDO );

		} else {
			apli_mensaje ("Comando no identificado.", portMAX_DELAY );
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
		if ( (65<=*PCARACTER && 90>=*PCARACTER)
			  || '/'==*PCARACTER                 ) {
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
	char * TEXTO = NULL;
	bool_t	DeboConfigurar = false;
	capturadora_config_s CCONFIG = {0};
	atributo_valor_t	AtributoValorTxt = {0};
	atributo_e			AtributoId = ATRIBUTO_NO_IDENTIFICADO;

	// Analizo subcomando

	// INICIAR
	if (comparar_texto(SUBCMD,CMD_INICIAR)) {
		CaptuRTOS_Comenzar(portMAX_DELAY);

	// PARAR
	} else if (comparar_texto(SUBCMD,CMD_PARAR)) {
		CaptuRTOS_Parar(portMAX_DELAY);

	// CONFIGURAR
	} else if (comparar_texto(SUBCMD,CMD_CONFIGURAR)) {
		TEXTO = subcomando( SUBCMD, CMD_CONFIGURAR );
		// Obtengo parámetros actuales
		CaptuRTOS_Obtener ( &CCONFIG, portMAX_DELAY );
		// Busco parámetros nuevos
		do {
			TEXTO = obtener_atributo( TEXTO, &AtributoId, AtributoValorTxt );
			if(AtributoId<NUM_ATRIBUTOS) {
				// apli_mensaje( ATRIBUTO_ID_TXT[AtributoId], portMAX_DELAY );
				// apli_mensaje( AtributoValorTxt, portMAX_DELAY );
				if(true==cargar_atributo_captu(&CCONFIG, AtributoId, AtributoValorTxt))
					DeboConfigurar = true;
			}
		} while(AtributoId<NUM_ATRIBUTOS);
		if(true==DeboConfigurar) {
			//apli_mensaje( "Intentamos configurar.", portMAX_DELAY );
			CaptuRTOS_Configurar( &CCONFIG, portMAX_DELAY );
		}
		//apli_mensaje( "Aun no puedo configurar Capturadora...", portMAX_DELAY );

	// OBTENER
	} else if (comparar_texto(SUBCMD,CMD_OBTENER)) {
		CaptuRTOS_EscribirConfiguraciones(portMAX_DELAY);

	// ENTRADA
	} else if (identificar_entrada(SUBCMD) <= ENTRADAS_TODAS) {
		cmd_entrada(SUBCMD);

	// Subcomando NO RECONOCIDO
	} else {
		apli_mensaje( "Comando de Capturadora no reconocido.", portMAX_DELAY );
	}
}

void cmd_entrada( char* COMANDO )
{
	// Variables locales
	char *	ATRIBUTOS = NULL;
	char *	SUBCMD = NULL;
	bool_t	DeboConfigurar = false;
	entrada_id_e	EntradaId = ENTRADA_NO_IDENTIFICADA;
	entrada_config_s	EntradaConfig = {0};
	atributo_valor_t	AtributoValorTxt = {0};
	atributo_e			AtributoId = ATRIBUTO_NO_IDENTIFICADO;

	// Identifico ENTRADA y apunto subcomando
	EntradaId = identificar_entrada ( COMANDO );
	if (EntradaId==ENTRADA_1) {
		SUBCMD = subcomando( COMANDO, CMD_ENTRADA_1 );
	} else if (EntradaId==ENTRADA_2) {
		SUBCMD = subcomando( COMANDO, CMD_ENTRADA_2 );
	} else if (EntradaId==ENTRADAS_TODAS) {
		SUBCMD = subcomando( COMANDO, CMD_ENTRADA_X );
	} else {
		apli_mensaje( "CAPTU ENTRADA no reconocida...", portMAX_DELAY );
		return;
	}

	// Analizo subcomando

	// ENCENDER
	if (comparar_texto(SUBCMD,CMD_ENCENDER)) {
		if(false==CaptuRTOS_EntradaEncender(EntradaId,portMAX_DELAY)) {
			apli_mensaje( "CAPTU No pudimos encender ENTRADA.", portMAX_DELAY );
		}
	}

	// APAGAR
	if (comparar_texto(SUBCMD,CMD_APAGAR)) {
		if(false==CaptuRTOS_EntradaApagar(EntradaId,portMAX_DELAY)) {
			apli_mensaje( "CAPTU No pudimos apagar ENTRADA.", portMAX_DELAY );
		}
	}

	// CONFIGURAR
	if (comparar_texto(SUBCMD,CMD_CONFIGURAR)) {
		ATRIBUTOS = subcomando( SUBCMD, CMD_CONFIGURAR );
		// Obtengo parámetros actuales
		if (ENTRADA_2==EntradaId) {
			CaptuRTOS_EntradaObtener ( ENTRADA_2, &EntradaConfig, portMAX_DELAY );
		} else {
			CaptuRTOS_EntradaObtener ( ENTRADA_1, &EntradaConfig, portMAX_DELAY );
		}
		// Busco atributos
		do {
			ATRIBUTOS = obtener_atributo( ATRIBUTOS, &AtributoId, AtributoValorTxt );
			if(AtributoId<NUM_ATRIBUTOS) {
				if(true==cargar_atributo_entrada(&EntradaConfig, AtributoId, AtributoValorTxt)) {
					DeboConfigurar = true;
				}
				//apli_mensaje( ATRIBUTO_ID_TXT[AtributoId], portMAX_DELAY );
				//apli_mensaje( AtributoValorTxt, portMAX_DELAY );
			}
		} while(AtributoId<NUM_ATRIBUTOS);
		if(true==DeboConfigurar) {
			//apli_mensaje( "Intentamos configurar.", portMAX_DELAY );
			CaptuRTOS_EntradaConfigurar( EntradaId, &EntradaConfig, portMAX_DELAY );
		}
	}
}

void cmd_generador( char * COMANDO )
{
	// Variables locales
	char *	SUBCMD = subcomando( COMANDO, CMD_GENERADOR );

	// Analizo subcomando
	// OBTENER
	if (comparar_texto(SUBCMD,CMD_OBTENER)) {
		GenRTOS_EscribirConfiguraciones(portMAX_DELAY);

	// SALIDA
	} else if (identificar_generador(SUBCMD) <= GENERADORES_TODOS) {
		cmd_salida(SUBCMD);

	// Subcomando NO RECONOCIDO
	} else {
		apli_mensaje( "Subomando GEN no reconocido.", portMAX_DELAY );
	}
}

void cmd_salida( char* COMANDO )
{
	// Variables locales
	char *		ATRIBUTOS = NULL;
	char *		SUBCMD = NULL;
	bool_t		DeboConfigurar = false;
	gen_id_e		GeneradorId = GENERADOR_NO_IDENTIFICADO;
	gen_conf_s	GeneradorConfig = {0};
	atributo_valor_t	AtributoValorTxt = {0};
	atributo_e	AtributoId = ATRIBUTO_NO_IDENTIFICADO;

	// Identifico Generador
	GeneradorId = identificar_generador( COMANDO );
	if (GeneradorId==GENERADOR_1) {
		SUBCMD = subcomando( COMANDO, CMD_SALIDA_1 );
	} else if (GeneradorId==GENERADOR_2) {
		SUBCMD = subcomando( COMANDO, CMD_SALIDA_2 );
	} else if (GeneradorId==GENERADORES_TODOS) {
		SUBCMD = subcomando( COMANDO, CMD_SALIDA_X );
	} else {
		apli_mensaje( "GEN SALIDA no reconocida...", portMAX_DELAY );
		return;
	}

	// Analizo subcomando

	// ENCENDER
	if (comparar_texto(SUBCMD,CMD_ENCENDER)) {
		if(false==GenRTOS_Encender(GeneradorId, portMAX_DELAY)) {
			apli_mensaje( "GEN No pudimos encender SALIDA.", portMAX_DELAY );
		}

	// APAGAR
	} else if (comparar_texto(SUBCMD,CMD_APAGAR)) {
		if(false==GenRTOS_Apagar(GeneradorId, portMAX_DELAY)) {
			apli_mensaje( "GEN No pudimos apagar SALIDA.", portMAX_DELAY );
		}

	// CONFIGURAR
	} else if (comparar_texto(SUBCMD,CMD_CONFIGURAR)) {
		ATRIBUTOS = subcomando( SUBCMD, CMD_CONFIGURAR );
		// Obtengo parámetros actuales
		if (GENERADOR_2==GeneradorId) {
			GenRTOS_Obtener ( GENERADOR_2, &GeneradorConfig, portMAX_DELAY );
		} else {
			GenRTOS_Obtener ( GENERADOR_1, &GeneradorConfig, portMAX_DELAY );
		}
		// Busco atributos
		if(GeneradorId<=CANTIDAD_GENERADORES) {
			do {
				ATRIBUTOS = obtener_atributo( ATRIBUTOS, &AtributoId, AtributoValorTxt );
				if(AtributoId<NUM_ATRIBUTOS) {
					if(true==cargar_atributo_gen(&GeneradorConfig, AtributoId, AtributoValorTxt))
						DeboConfigurar = true;
					//apli_mensaje( ATRIBUTO_ID_TXT[AtributoId], portMAX_DELAY );
					//apli_mensaje( AtributoValorTxt, portMAX_DELAY );
				}
			} while(AtributoId<NUM_ATRIBUTOS);
			if(true==DeboConfigurar) {
				GenRTOS_Configurar( GeneradorId,&GeneradorConfig, portMAX_DELAY );
			}
		}

	// OBTENER
	}
	/*else if (comparar_texto(SUBCMD,CMD_OBTENER)) {
		GenRTOS_EscribirConfiguraciones(portMAX_DELAY);*/
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
	} else if (comparar_texto(TEXTO,CMD_SALIDA_2)) {
		Generador = GENERADOR_2;
	} else if (comparar_texto(TEXTO,CMD_SALIDA_X)) {
		Generador = GENERADORES_TODOS;
	}
	if(Generador==GENERADOR_NO_IDENTIFICADO)
		apli_mensaje( "Generador no identificado.", portMAX_DELAY );
	return Generador;
}

entrada_id_e identificar_entrada ( char * TEXTO )
{
	entrada_id_e Entrada = ENTRADA_NO_IDENTIFICADA;
	if (comparar_texto(TEXTO,CMD_ENTRADA_1)) {
		Entrada = ENTRADA_1;
	} else if (comparar_texto(TEXTO,CMD_ENTRADA_2)) {
		Entrada = ENTRADA_2;
	} else if (comparar_texto(TEXTO,CMD_ENTRADA_X)) {
		Entrada = ENTRADAS_TODAS;
	}
	if(Entrada==ENTRADA_NO_IDENTIFICADA)
		apli_mensaje( "ENTRADA no identificada.", portMAX_DELAY );
	return Entrada;
}

char* obtener_atributo( char * TEXTO, atributo_e * P_ATRIB_HALLADO, atributo_valor_t ATRIB_VALOR )
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
	case TIPO:
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

bool cargar_atributo_captu( capturadora_config_s * CCONFIG, atributo_e ATRID, atributo_valor_t ATRVLR )
{
	// Variables locales:
	bool		RET = false;
	double	NUM = 0;
	char*		RETCON = NULL;

	// Evalúo:
	switch (ATRID) {
	case ESCALA:
		NUM = strtod(ATRVLR, &RETCON);
		if(NUM>0) {
			CCONFIG->EscalaHorizontal = NUM;
			RET = true;
		}
		break;

	case ORIGEN:
		if       (0==strcmp(CMD_ENTRADA_1, ATRVLR)) {
			CCONFIG->OrigenDisparo = ENTRADA_1;
			RET = true;
		} else if(0==strcmp(CMD_ENTRADA_2, ATRVLR)) {
			CCONFIG->OrigenDisparo = ENTRADA_2;
			RET = true;
		}
		break;

	case PROMEDIO:
		NUM = strtod(ATRVLR, &RETCON);
		if(NUM>=0) {
			RET=true;
			CCONFIG->ModoCaptura = CAPTURA_UNICA;
			if(NUM>=16) {
				CCONFIG->ModoCaptura |= CAPTURA_PROMEDIADA_16;
			} else if(NUM>=4) {
				CCONFIG->ModoCaptura |= CAPTURA_PROMEDIADA_4;
			}
		}
		break;

	default:
		RET = false;
	}

	// Chau!
	return RET;
}

bool cargar_atributo_entrada( entrada_config_s * PECONFIG, atributo_e ATRID, atributo_valor_t ATRVLR )
{
	// Variables locales:
	bool		RET = false;
	double	NUM = 0;
	char*		RETCON = NULL;

	// Evalúo:
	switch (ATRID) {
	case ESCALA:
		NUM = strtod(ATRVLR, &RETCON);
		if(NUM>0) {
			PECONFIG->EscalaVertical = NUM;
			RET = true;
		}
		break;

	case FLANCO:
		if (0==strcmp("SUBIDA", ATRVLR)) {
			PECONFIG->FlancoDisparo = SUBIDA;
			RET = true;
		} else if(0==strcmp("BAJADA", ATRVLR)) {
			PECONFIG->FlancoDisparo = BAJADA;
			RET = true;
		}
		break;

	case NIVEL:
		NUM = strtod(ATRVLR, &RETCON);
		if(*RETCON == '\0') {
			PECONFIG->NivelDisparo = NUM;
			RET = true;
		}
		break;

	default:
		RET = false;
	}

	// Chau!
	return RET;
}

