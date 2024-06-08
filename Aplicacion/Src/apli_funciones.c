/*******************************************************************************
  * @file           : desarrollo.c
  * @brief          : Funciones en desarrollo
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
	uoEscribirTxt3 ( "ALERTA ", TEXTO, "\n\r");
}

BaseType_t apli_mensaje ( const char * TEXTO, TickType_t ESPERA )
{
	BaseType_t RET;
	RET = tomar_escritura(ESPERA);
	if( pdTRUE == RET ) {
		uoEscribirTxt2 ("MSJ ", TEXTO);
		devolver_escritura();
	}
	return RET;
}

BaseType_t tomar_escritura ( TickType_t ESPERA )
{
	return xSemaphoreTake( MutexApliEscribir, ESPERA );
}

BaseType_t devolver_escritura ( void )
{
	return xSemaphoreGive( MutexApliEscribir );
}
