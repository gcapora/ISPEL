/**************************************************************************************************
* @file    uleds.c
* @author  Guillermo Caporaletti
* @brief   Módulo para manejo de leds con diversos modos de encendido
* @date	   julio de 2023
*
**************************************************************************************************/

/****** Librerías (includes) *********************************************************************/

#include <uLeds.h>

/****** Definiciones privadas (macros) ***********************************************************/

#define DURACION_CICLO_SUSPENSION 40	// ms que dura cada ciclo de trabajo en modo SUSPENSION

/****** Definiciones privadas de tipos de datos (private typedef) ********************************/

typedef struct {
	hal_pin_id_t	pinLed;			// identificador de pin según uHAL.h
    bool			inicializado;
    led_estado_t	estado;			// estado lógico del led: encendido en algún modo o apagado
    led_modo_t		modo;
    int16_t     	nEncendido;
    int16_t     	nApagado;
    int16_t     	nActual;
    uint16_t		CuentaModoSuspension;
    int8_t			SentidoCuentaModoSuspension;
} led_control_t;

/****** Definición de datos privados *************************************************************/

const uint16_t	CiclosEncendidos	[CANTIDAD_MODOS_LED] = { 10, 6, 3, 1, 1, 200, 750, 100, 1 };
const uint16_t	CiclosApagados		[CANTIDAD_MODOS_LED] = {  0, 4, 7, 9, 19, 200, 750, 1400, 19 };
led_control_t	vectorLedControl	[CANTIDAD_LED] = {0};

/****** Declaración de funciones privadas ********************************************************/

bool uLedVerificarId(led_id_t);

/****** Definición de funciones privadas *********************************************************/

/*-------------------------------------------------------------------------------------------------
 * @brief    Verifica que id_led sea válido
 * @param    id_led
 * @retval
 */
bool uLedVerificarId(led_id_t led_id) {
    if ( led_id < 0) return false;
    if ( led_id >= CANTIDAD_LED) return false;
    if ( vectorLedControl[led_id].inicializado == false ) return false;
    return true;
}

/****** Definición de funciones públicas *********************************************************/

/*-------------------------------------------------------------------------------------------------
 * @brief
 * @param
 * @retval   Un identificador para manejar el led inicializado
 */
led_id_t uLedInicializar(hal_pin_id_t pin_led) {
    led_id_t IdLed = 0;
    bool Id_Establecido = false;
    hal_pin_config_s pin_config = {0};

    // Verifico que este gpio_t pin_led no haya sido inicializado antes
    for (IdLed = 0; IdLed<CANTIDAD_LED; IdLed++) {
        if ( vectorLedControl[IdLed].pinLed == pin_led && 
             vectorLedControl[IdLed].inicializado == true ) {
            // Ya lo habíamos inicializado...
            // Ya tengo el valor de IdLed adecuado.
            Id_Establecido = true;
            break;
        }
    }

    // Si no había sido inicializado, busco Id libre
    if ( false == Id_Establecido ) {
        for (IdLed = 0; IdLed<CANTIDAD_LED; IdLed++) {
            if ( vectorLedControl[IdLed].inicializado == false ) {
                // Encontramos un Id libre
                Id_Establecido = true;
                break;
            }
        }
    }

    if ( Id_Establecido == false ) return ERROR_LED;

    // Reseteamos valores
    vectorLedControl[IdLed].pinLed = pin_led;
    vectorLedControl[IdLed].estado = LED_APAGADO;
    vectorLedControl[IdLed].modo = PREDETERMINADO;
    vectorLedControl[IdLed].nActual = 0;
    vectorLedControl[IdLed].nEncendido = CiclosEncendidos[vectorLedControl[IdLed].modo];
    vectorLedControl[IdLed].nApagado = CiclosApagados[vectorLedControl[IdLed].modo];
    vectorLedControl[IdLed].SentidoCuentaModoSuspension = 1;
    vectorLedControl[IdLed].CuentaModoSuspension = 0;
    vectorLedControl[IdLed].inicializado = true;

    // Inicializamos pin gpio
    pin_config.Modo = U_GPIO_MODO_SALIDA;
    pin_config.Tirar = U_GPIO_NO_TIRAR;
    pin_config.Velocidad = U_GPIO_VELOCIDAD_BAJA;
    uHALgpioInicializar ( pin_led, &pin_config );
    uHALgpioEscribir    ( pin_led, false );

    // Inicialización exitosa
    return IdLed;
}

/*-------------------------------------------------------------------------------------------------
 * @brief
 * @param
 * @retval
 */
bool uLedEncender(led_id_t led_id) {
    if ( false == uLedVerificarId(led_id) ) return false;
    vectorLedControl[led_id].nActual = 0;
    vectorLedControl[led_id].estado = LED_ENCENDIDO;
    if (false == uLedActualizar(led_id) ) return false;
    return true;
}

/*-------------------------------------------------------------------------------------------------
 * @brief
 * @param
 * @retval
 */
bool uLedApagar(led_id_t led_id) {
    if ( false == uLedVerificarId(led_id) ) return false;
    vectorLedControl[led_id].estado = LED_APAGADO;
    if (false == uLedActualizar(led_id) ) return false;
    return true;
}

/*-------------------------------------------------------------------------------------------------
 * @brief
 * @param
 * @retval
 */
bool uLedActualizar(led_id_t led_id) {
	// Verificar parámetros
	if ( false == uLedVerificarId(led_id) ) return false;

    // Variables locales
	bool salida = false;
	uint16_t periodo = vectorLedControl[led_id].nEncendido + vectorLedControl[led_id].nApagado;

    // Determino estado físico de salida
    if ( LED_APAGADO == vectorLedControl[led_id].estado) {
        // Led apagado
        salida = false;
    } else if ( vectorLedControl[led_id].nActual < vectorLedControl[led_id].nEncendido ) {
        // Led estado-encendido dentro del ciclo-encendido
        salida = true;
    } else {
        salida = false;
    }

    // Escribo y actualizo estado físico
    uHALgpioEscribir (vectorLedControl[led_id].pinLed, salida);

    // Actualizo fase del período
    vectorLedControl[led_id].nActual++;
    if ( vectorLedControl[led_id].nActual >= 
        (vectorLedControl[led_id].nEncendido + vectorLedControl[led_id].nApagado) ) {
        // Se cumplió un período de ciclos completo
        vectorLedControl[led_id].nActual = 0;
    }

    // Analizo si debo cambiar ciclo de trabajo
    if ( vectorLedControl[led_id].modo == SUSPENSION ) {
    	// Sumo un N dentro del ciclo y veo si cambio intensidad
    	if (++vectorLedControl[led_id].CuentaModoSuspension > DURACION_CICLO_SUSPENSION ) {
    		// Debo cambiar intensidad...
    		vectorLedControl[led_id].CuentaModoSuspension = 0;	// Reseteo cuenta
    		vectorLedControl[led_id].nEncendido += vectorLedControl[led_id].SentidoCuentaModoSuspension;
    		vectorLedControl[led_id].nApagado -= vectorLedControl[led_id].SentidoCuentaModoSuspension;
    		if ( vectorLedControl[led_id].nApagado < 0 ) {
    			vectorLedControl[led_id].nApagado = 0;
    			vectorLedControl[led_id].nEncendido = periodo;
    			vectorLedControl[led_id].SentidoCuentaModoSuspension = -1;
    		}
    		if ( vectorLedControl[led_id].nEncendido < 0 ) {
    		    vectorLedControl[led_id].nApagado = periodo;
    		    vectorLedControl[led_id].nEncendido = 0;
    			vectorLedControl[led_id].SentidoCuentaModoSuspension = 1;
    		}
    	}
    }

    // Listo!!!
    return true;
}

/*-------------------------------------------------------------------------------------------------
 * @brief
 * @param
 * @retval
 */
bool uLedActualizarTodo ( void ) {
	bool algo_actualizado = false;
	for (led_id_t i=0; i<CANTIDAD_LED; i++) {
		if ( true == uLedActualizar(i) ) {
			algo_actualizado = true;
		}
	}
	return algo_actualizado;
}
/*-------------------------------------------------------------------------------------------------
 * @brief
 * @param
 * @retval
 */
bool uLedConfigurarModo(led_id_t led_id, led_modo_t led_modo) {
    if ( false == uLedVerificarId(led_id) ) return false;
    vectorLedControl[led_id].modo = led_modo;
    vectorLedControl[led_id].nEncendido = CiclosEncendidos[vectorLedControl[led_id].modo];
    vectorLedControl[led_id].nApagado = CiclosApagados[vectorLedControl[led_id].modo];
    return true;
}

/*-------------------------------------------------------------------------------------------------
 * @brief
 * @param
 * @retval
 */
led_estado_t uLedObtenerEstado(led_id_t led_id) {
    if ( false == uLedVerificarId(led_id) ) return false;
    return vectorLedControl[led_id].estado;
}

/*-------------------------------------------------------------------------------------------------
 * @brief   Obtiene el modo de operación del led
 * @param   led id
 * @retval  modo del led
 */
led_modo_t uLedObtenerModo(led_id_t led_id) {
    if ( false == uLedVerificarId(led_id) ) return (led_modo_t) ERROR_MODO_LED;
    return vectorLedControl[led_id].modo;
}

/*-------------------------------------------------------------------------------------------------
 * @brief
 * @param
 * @retval
 */
bool uLedDesinicializar(led_id_t led_id) {
    if ( false == uLedVerificarId(led_id) ) return false;
    vectorLedControl[led_id].inicializado = false;
    return true;
}

/*-------------------------------------------------------------------------------------------------
 * @brief
 * @param
 * @retval
 */
led_estado_t uLedObtenerEstadoFisico(led_id_t led_id) {
    if ( false == uLedVerificarId(led_id) ) return false;
    return (led_estado_t) uHALgpioLeer ( vectorLedControl[led_id].pinLed );
}

/****************************************************************** FIN DE ARCHIVO ***************/
