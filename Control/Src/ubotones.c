/**************************************************************************************************
* @file    ubotones.c
* @author  Guillermo Caporaletti
* @brief   Módulo para manejo de botones con antirrebote
* @date	   agosto de 2023
*
**************************************************************************************************/

/****** Librerías (includes) *********************************************************************/

#include "apli.h"	// TODO sacar esto cuando haya implementado uOSAL
#include "ubotones.h"

/****** Definiciones privadas (macros) ***********************************************************/

#define TIEMPO_ESPERA_ANTIRREBOTE	30	// ms que se espera antes de confirmar cambio de estado.
#define TIEMPO_PRESIONADO_LARGO		500	// ms que se contabilizan para definir un presionado largo.

/****** Definiciones privadas de tipos de datos (private typedef) ********************************/

typedef enum {
	 BOTON_NO_PRESIONADO,
	 BOTON_A_CONFIRMAR_PRESIONADO,
	 BOTON_PRESIONADO,
	 BOTON_A_CONFIRMAR_NO_PRESIONADO
}				boton_estado_t;
typedef struct {
	hal_pin_id_t	pinBoton;			// identificador de pin según uHAL.h
    bool			inicializado;
    boton_estado_t	estado;				// estado del boton
    bool			FlancoPresionadoDetectado;
    bool			FlancoNoPresionadoDetectado;
    bool			PresionadoLargoDetectado;
    uint32_t		TiempoCambioDetectado;
}				boton_control_t;

/****** Definición de datos privados *************************************************************/

boton_control_t	vectorBotonControl	[CANTIDAD_BOTONES] = {0};

/****** Declaración de funciones privadas ********************************************************/

bool uBotonVerificarId(boton_id_t);

/****** Definición de funciones privadas *********************************************************/

/*-------------------------------------------------------------------------------------------------
 * @brief	Verifica que ID sea válido
 * @param   boton_id_t ID
 * @retval	true si ID existe y ha sido inicializado
 */
bool uBotonVerificarId(boton_id_t ID) {
    if ( ID < 0) return false;
    if ( ID >= CANTIDAD_BOTONES) return false;
    if ( vectorBotonControl[ID].inicializado == false ) return false;
    return true;
}

/****** Definición de funciones públicas *********************************************************/

/*-------------------------------------------------------------------------------------------------
 * @brief	Inicializa o crea un manejador de un botón
 * @param	PIN = identificador de hardware del pin
 * @retval  Un identificador para manejar el led inicializado
 */
boton_id_t uBotonInicializar(hal_pin_id_t PIN) {
    boton_id_t IdBoton = 0;
    bool Id_Establecido = false;
    hal_pin_config_s pin_config = {0};

    // Verifico que este gpio_t pin_led no haya sido inicializado antes
    for (IdBoton = 0; IdBoton < CANTIDAD_BOTONES; IdBoton++) {
        if ( vectorBotonControl[IdBoton].pinBoton == PIN &&
             vectorBotonControl[IdBoton].inicializado == true ) {
            // Ya lo habíamos inicializado...
            // Ya tengo el valor de IdBoton adecuado.
            Id_Establecido = true;
            break;
        }
    }

    // Si no había sido inicializado, busco Id libre
    if ( false == Id_Establecido ) {
        for (IdBoton = 0; IdBoton < CANTIDAD_BOTONES; IdBoton++) {
            if ( vectorBotonControl[IdBoton].inicializado == false ) {
                // Encontramos un Id libre
                Id_Establecido = true;
                break;
            }
        }
    }

    if ( Id_Establecido == false ) return ERROR_BOTON; // No hubo Id libre... :-(

    // Establecemos valores iniciales
    vectorBotonControl[IdBoton].pinBoton = PIN;
    vectorBotonControl[IdBoton].estado = BOTON_NO_PRESIONADO;
    vectorBotonControl[IdBoton].FlancoNoPresionadoDetectado = false;
    vectorBotonControl[IdBoton].FlancoPresionadoDetectado = false;
    vectorBotonControl[IdBoton].PresionadoLargoDetectado = false;
    vectorBotonControl[IdBoton].TiempoCambioDetectado = 0;
    vectorBotonControl[IdBoton].inicializado = true;

    // Inicializamos pin gpio
    pin_config.Modo = U_GPIO_MODO_ENTRADA;
    pin_config.Tirar = U_GPIO_TIRAR_ABAJO;
    pin_config.Velocidad = U_GPIO_VELOCIDAD_BAJA;
    uHALgpioInicializar ( PIN, &pin_config );

    // Inicialización exitosa
    return IdBoton;
}

/*-------------------------------------------------------------------------------------------------
 * @brief	Desinicializa o destruye el manejador de un botón
 * @param	ID = manejador del boton previamente inicializado
 * @retval	true si pudo desinicializar, false si no era posible
 */
bool uBotonDesinicializar(boton_id_t ID) {
    if ( false == uBotonVerificarId(ID) ) return false;
    vectorBotonControl[ID].inicializado = false;
    return true;
}

/*-------------------------------------------------------------------------------------------------
 * @brief	Actualiza la MEF del botón ID
 * @param	ID = manejador del boton
 * @retval	true si pudo actualizar MEF
 */
bool uBotonActualizar(boton_id_t ID) {
	// Verificar parámetros
	if ( false == uBotonVerificarId(ID) ) return false;

    // Variables locales
	bool LecturaBoton = false;
	uint32_t TiempoLectura = 0;

    // Leo pin-boton
    LecturaBoton = uHALgpioLeer (vectorBotonControl[ID].pinBoton);
    TiempoLectura = (uint32_t) xTaskGetTickCount();	// TODO pasarlo a uOSAL

    // Analizo estado y decido ------------------------------------------------
    switch (vectorBotonControl[ID].estado) {

    		case BOTON_NO_PRESIONADO:	//-------------------------------------
    			if (LecturaBoton == true) {
    				// Debo cambiar de estado...
    				vectorBotonControl[ID].estado = BOTON_A_CONFIRMAR_PRESIONADO;
    				vectorBotonControl[ID].TiempoCambioDetectado = TiempoLectura;
    			}
    			break;

    		case BOTON_A_CONFIRMAR_PRESIONADO:	//-----------------------------
    			if ( false == LecturaBoton ) {
    				// Fue un falso BOTON_PRESIONADO
    				vectorBotonControl[ID].estado = BOTON_NO_PRESIONADO;

    			} else if ( TiempoLectura - vectorBotonControl[ID].TiempoCambioDetectado >=
    					    TIEMPO_ESPERA_ANTIRREBOTE ) {
    				// Hay un cambio de estado!!!
    				vectorBotonControl[ID].estado = BOTON_PRESIONADO;
    				vectorBotonControl[ID].TiempoCambioDetectado = TiempoLectura;
    				vectorBotonControl[ID].FlancoPresionadoDetectado = true;
    				//vectorBotonControl[ID].FlancoNoPresionadoDetectado = false;
    			}
    			break;

    		case BOTON_PRESIONADO:	//-----------------------------------------
    			if (LecturaBoton == false) {
    				// Debo cambiar de estado...
       				vectorBotonControl[ID].estado = BOTON_A_CONFIRMAR_NO_PRESIONADO;
        			vectorBotonControl[ID].TiempoCambioDetectado = TiempoLectura;
        		} else if (( TiempoLectura - vectorBotonControl[ID].TiempoCambioDetectado
        				     >= TIEMPO_PRESIONADO_LARGO ) &&
						   ( vectorBotonControl[ID].TiempoCambioDetectado != 0 )) {
        			// Hay un presionado largo!!!
       				vectorBotonControl[ID].PresionadoLargoDetectado = true;
       				vectorBotonControl[ID].TiempoCambioDetectado = 0;
       					// Esto es para que lo detecte solo una vez
        		}
        		break;

    		case BOTON_A_CONFIRMAR_NO_PRESIONADO:	//-------------------------
    			if ( true == LecturaBoton ) {
    				// Fue un falso BOTON_NO_PRESIONADO
    				vectorBotonControl[ID].estado = BOTON_PRESIONADO;

    			} else if ( TiempoLectura - vectorBotonControl[ID].TiempoCambioDetectado >=
    					    TIEMPO_ESPERA_ANTIRREBOTE ) {
    				// Hay un cambio de estado!!!
    				vectorBotonControl[ID].estado = BOTON_NO_PRESIONADO;
    				vectorBotonControl[ID].TiempoCambioDetectado = TiempoLectura;
    				//vectorBotonControl[ID].FlancoPresionadoDetectado = false;
    				//vectorBotonControl[ID].PresionadoLargoDetectado = false;
    				vectorBotonControl[ID].FlancoNoPresionadoDetectado = true;
    			}
    			break;

    		default:	//-----------------------------------------------------
    			configASSERT(false);
    		break;

    } // Fin de switch (vectorBotonControl[ID].estado)

    return true;
}

/*-------------------------------------------------------------------------------------------------
 * @brief	Actualiza las MEFs de todos los botones
 * @param	Ninguno
 * @retval	true si pudo actualizar algún botón
 */
bool uBotonActualizarTodo ( void ) {
	bool algo_actualizado = false;
	for (boton_id_t i=0; i<CANTIDAD_BOTONES; i++) {
		if ( true == uBotonActualizar(i) ) {
			algo_actualizado = true;
		}
	}
	return algo_actualizado;
}

/*-------------------------------------------------------------------------------------------------
 * @brief	Verifica si un botón está NO PRESIONADO
 * @param	ID = manejador del botón
 * @retval	true si el botón está NO PRESIONADO
 */
bool uBotonNoPresionado (boton_id_t ID) {
	if ( BOTON_NO_PRESIONADO == vectorBotonControl[ID].estado ) {
		// vectorBotonControl[ID].NoPresionadoDetectado = false;
		return true;
	}
	return false;
}

/*-------------------------------------------------------------------------------------------------
 * @brief	Verifica si se produjo un FLANCO hacia botón NO PRESIONADO
 * @param	ID = manejador del botón
 * @retval	true si hubo un FLANCO hacia boón NO PRESIONADO
 * 			Sólo un true por un flanco detectado
 */
bool uBotonFlancoNoPresionado (boton_id_t ID) {
	if ( true == vectorBotonControl[ID].FlancoNoPresionadoDetectado ) {
		vectorBotonControl[ID].FlancoNoPresionadoDetectado = false;
		return true;
	}
	return false;
}

/*-------------------------------------------------------------------------------------------------
 * @brief	Verifica si un botón está PRESIONADO
 * @param	ID = manejador del botón
 * @retval	true si el botón está PRESIONADO
 */
bool uBotonPresionado (boton_id_t ID) {
	if ( BOTON_PRESIONADO == vectorBotonControl[ID].estado  ) {
		//vectorBotonControl[ID].FlancoPresionadoDetectado = false;
		return true;
	}
	return false;
}

/*-------------------------------------------------------------------------------------------------
 * @brief	Verifica si se detectó un BOTON PRESIONADO LARGO
 * @param	ID = manejador del botón
 * @retval	true si el botón fue PRESIONADO durante más de TIEMPO_PRESIONADO_LARGO milisegundos
 * 			Sólo un true por un presionado largo detectado
 */
bool uBotonPresionadoLargo (boton_id_t ID) {
	if ( true == vectorBotonControl[ID].PresionadoLargoDetectado ) {
		vectorBotonControl[ID].PresionadoLargoDetectado = false;
		return true;
	}
	return false;
}

/*-------------------------------------------------------------------------------------------------
 * @brief	Verifica si se produjo un FLANCO hacia botón PRESIONADO
 * @param	ID = manejador del botón
 * @retval	true si hubo FLANCO hacia boón PRESIONADO
 * 			Sólo un true por un flanco detectado
 */
bool uBotonFlancoPresionado (boton_id_t ID) {
	if ( true == vectorBotonControl[ID].FlancoPresionadoDetectado ) {
		vectorBotonControl[ID].FlancoPresionadoDetectado = false;
		return true;
	}
	return false;
}

/****************************************************************** FIN DE ARCHIVO ***************/
