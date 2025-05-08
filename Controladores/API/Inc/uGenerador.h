/**************************************************************************************************
 * Archivo: uGenerador.h
 * Breve:	Generador de seniales visto desde el usuario para proyecto ISPEL.
 * Fecha:	Julio de 2024
 * Autor:	Guillermo F. Caporaletti
 * Version: v1
 *
 * Descripción:
 *  - Desarrolla el objeto Generador visto desde el operario.
 *  - Define una estructura gen_config_s con los parámetros de la configuración deseada.
 *  - Define tipos de datos enumeración para identificar generadores, acople y estado de generador.
 *  - Las funciones inicializan, configuran, encienden y apagan el generador. También puede 
 *    devolver información sobre su propia configuración y la señal cargada.
 *
 * Para mejorar:
 *  - Almacenamos 4 bytes por muestra en la senial aunque usamos solo 12 bits. Debe probrarse
 *    utilizando alineamiento derecha e izquierda de un mismo vector de 32 bits para almacenar dos
 *    señales. Esto bajaría a la mitad la memoria utilizada por canal.
 *  - Analizar si son necesarios los campos Divisor y Largo de la estructura gen_conf_s.
 *
 *************************************************************************************************/

#ifndef ISPEL_UGENERADOR_H_
#define ISPEL_UGENERADOR_H_

/****** Librerías (includes) *********************************************************************/

#include "uOSAL.h"
#include "uHAL.h"
#include "uSeniales.h"

/****** Definiciones públicas (macros) ***********************************************************/

#define UG_CANTIDAD_MUESTRAS_SENIAL    5000

/****** Definiciones públicas de tipos de datos (public typedef) *********************************/

// Identificación del generador
typedef enum {
	GENERADOR_1,
	GENERADOR_2,
	CANTIDAD_GENERADORES,
	GENERADORES_TODOS = CANTIDAD_GENERADORES,
	GENERADOR_NO_IDENTIFICADO
} gen_id_e;

// Acople del generador
typedef enum {
	DC,
	AC,
	CERO
} gen_acople_e;

// Estructura para configurar a un generador
typedef struct {
	senial_tipo  Tipo;
	float        Maximo;       // en voltios
	float        Minimo;       // en voltios
	double       Frecuencia;   // de la señal en Hertz
	uint32_t     Divisor;      // de muestreo: establece por cuánto se divide la frecuancia base
	float     	 Largo;        // cantidad de muestras de la señal resultante (solo salida)
	float        Fase;         // en grados, entre 0º y 360º
	float        Simetria;     // número entre 0 y 1 (no aplica en senoidal)
	gen_acople_e Acople;
} gen_conf_s;

/* Relación entre Frecuencia (de la señal) y Divisor:
   - Para configurar se debe usar uno de los dos.
   - Para utilizar Divisor, se debe dejar Frecuencia = 0.
   - Divisor establece por cuánto se divide a la frecuencia base para determinar la frecuencia de muestreo.
   - La frecuencia de la señal es frecuencia de muestreo / largo de la señal
*/

// Estados del generador
typedef enum {
	GENERADOR_NO_INICIALIZADO,
	GENERADOR_APAGADO,
	GENERADOR_ENCENDIDO,
	GENERADOR_EN_ERROR,
} gen_estados_e;

/****** Declaraciones de datos externos **********************************************************/


/****** Declaración de funciones públicas ********************************************************/

bool uGeneradorInicializar (gen_id_e);
bool uGeneradorConfigurar  (gen_id_e, gen_conf_s *);
bool uGeneradorObtener     (gen_id_e, gen_conf_s *);
gen_estados_e uGeneradorObtenerEstado (gen_id_e);
bool uGeneradorEncender    (gen_id_e);
bool uGeneradorApagar      (gen_id_e);

/*************************************************************************************************/
#endif /* ISPEL_UGENERADOR_H_ */
/****************************************************************** FIN DE ARCHIVO ***************/
