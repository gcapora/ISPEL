/*******************************************************************************
  * @file           apli_funciones.c
  * @brief          
  ******************************************************************************
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include <math.h>
#include "apli.h"

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Variables importadas ------------------------------------------------------*/

//extern senial_s * P_Senial_E1;
//extern senial_s * P_Senial_E2;

/* Private function prototypes -----------------------------------------------*/

/* Public function ************************************************************/

void apli_alerta ( const char * TEXTO )
{
	uoEscribirTxt3 ( "ALERTA ", TEXTO, "\r\n");
}

BaseType_t apli_mensaje ( const char * TEXTO, TickType_t ESPERA )
{
	BaseType_t RET;
	RET = tomar_escritura(ESPERA);
	if( pdTRUE == RET ) {
		uoEscribirTxt2 ("MSJ ", TEXTO);
		uoEscribirTxt ("\r\n");
		devolver_escritura();
	}
	return RET;
}

void apli_separador(const char * SEPARA)
{
	static uint32_t tiempo_anterior = 0;
	if (uoMilisegundos()-tiempo_anterior >= TIEMPO_SEPARADOR) {
		tiempo_anterior += TIEMPO_SEPARADOR;
		tomar_escritura(portMAX_DELAY);
		uoEscribirTxt(SEPARA);
		devolver_escritura();
	}
}

void apli_latido(bool_t forzar)
{
	static uint32_t tiempo_ultimo = 0;
	if ( true == forzar ) {
		tiempo_ultimo = mensaje_latido();

	} else if (true == LatidoEncendido && (uoMilisegundos()-tiempo_ultimo) > TIEMPO_LATIDO ) {
		tomar_escritura    ( 3 * UN_SEGUNDO );
		tiempo_ultimo = mensaje_latido();
		devolver_escritura ();
	}
}

uint32_t mensaje_latido(void)
{
	uint32_t tiempo = uoMilisegundos();
	if (true==EquipoEncendido) {
		uoEscribirTxt  ("// Aca ISPEL encendido. ");
	} else {
		uoEscribirTxt  ("// Aca ISPEL en espera. ");
	}
	uoEscribirTxtUint  ("[", tiempo / 60000);
	uoEscribirTxtUint  (" min ", (tiempo/1000) % 60);
	uoEscribirTxt      (" s]\r\n");
	return tiempo;
}

BaseType_t tomar_escritura ( TickType_t ESPERA )
{
	return xSemaphoreTake( MutexApliEscribir, ESPERA );
}

BaseType_t devolver_escritura ( void )
{
	return xSemaphoreGive( MutexApliEscribir );
}


