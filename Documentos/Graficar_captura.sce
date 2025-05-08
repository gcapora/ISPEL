/*******************************************************************************
* ISPEL: Gráfica de los datos capturados (formateado en lineas)
********************************************************************************
* Breve: 
* Se interpreta un archivo de texto con los datos envados por el sistema 
* embebido y se grafica.
*
* Precondición:
* Se presuponen ambas entradas encendidas.
*
* Abril de 2025.
*******************************************************************************/

/*******************************************************************************
* Variables locales y definiciones
*******************************************************************************/

VERDADERO = %t;
FALSO = %f;
LF = ascii(10);
CR = ascii(13);
TAB = ascii(9);
LeyendoDatos = FALSO;
EsDato = FALSO;
Leer = VERDADERO;
Linea="";
L2="";
N = 1;

// Copiamos manualmente los datos del encabezado
// (vale la última cargada)

Archivo_de_datos = 'Captura_N02_24-07-10.txt';
FM = 2400000;
MUESTRAS = 6000;
DISPARO = 1500;
E1_VMAX = 3.3;
E1_VMIN = 0;
E2_VMAX = 3.3;
E2_VMIN = 0;

Archivo_de_datos = 'Captura_N04_25_cuadrada.txt';
FM = 2400000;
MUESTRAS =  4000;
DISPARO  =  2000;
E1_VMAX  =  11.363;
E1_VMIN  = -11.197;
E2_VMAX  =  11.504;
E2_VMIN  = -11.281;

Archivo_de_datos = 'Captura_N05_25_senoidal.txt';
FM = 2400000;
MUESTRAS =  4000;
DISPARO  =  2000;
E1_VMAX  =  11.363;
E1_VMIN  = -11.197;
E2_VMAX  =  11.504;
E2_VMIN  = -11.281;

Entrada1 = zeros(1,MUESTRAS);
Entrada2 = zeros(1,MUESTRAS);

/*******************************************************************************
* Funciones privadas
*******************************************************************************/

// Identifica separador de comandos, atributos y datos
function logico = es_separador(car)
    logico = FALSO;
    car = part(car,1:1);
    if ((car==' ') || (car==char(9)) || (car==',')) then
        logico = VERDADERO;
    end
endfunction 

// Identifica comentarios y comando CAPTU
function logico = es_comentario(str)
    logico = FALSO;
    str = convstr(str, "u");
    if ((length(str)>2) && (part(str, 1:2)=="//")) then
        if (es_separador(part(str, 3:3))) then
            logico = VERDADERO;
        end
    end
    if ((length(str)>3) && (part(str, 1:3)=="MSJ")) then
        if (es_separador(part(str, 4:4))) then
            logico = VERDADERO;
        end
    end
    if (((length(str)>5) && part(str, 1:5)=="CAPTU")) then
        if (es_separador(part(str, 6:6))) then
            logico = VERDADERO;
        end
    end
endfunction

// Analiza contenido de una linea
function [num, d1, d2] = analizo_linea(str)
    global LeyendoDatos; 
    global N;
    global MUESTRAS;
    num = 0;
    d1 = 0;
    d2 = 0;
    // Si es un comentario escribo la línea
    if (es_comentario(str)) then
        mprintf("%s\n", str);
    end
    // Debo analizar dato?
    if (LeyendoDatos == VERDADERO) then
        if (isnum(part(str,1))) then
            separador = strindex(str,' ');
            if (separador==[]) then
               separador = strindex(str,char(9));
            end
            if (separador==[]) then
               separador = strindex(str,',');
            end
            largo     = length(str);
            num = N;
            d1 = strtod(part(str,1:(separador-1)))     * (E1_VMAX-E1_VMIN) / 4096 + E1_VMIN;
            d2 = strtod(part(str,(separador+1):largo)) * (E2_VMAX-E2_VMIN) / 4096 + E2_VMIN;
            N = N+1;
            if (N>MUESTRAS) then
                LeyendoDatos = FALSO;
                mprintf("No puedo leer más datos datos.\n");
            end
        end
    end
    // ¿Es linea previa a dato?
    if (part(str, 1:6)=="DATOS ") then
        LeyendoDatos = VERDADERO;
        N=1;
        mprintf("Datos...\n");
    end
    // ¿Terminaron los datos?
    if (strindex(str,'}')~=[]) then
        LeyendoDatos = FALSO;
        mprintf("Finalizaron los datos.\n");
    end
    
endfunction

/*******************************************************************************
* Programa principal
*******************************************************************************/

// Abrimos archivo de datos ----------------------------------------------------
close();
mprintf('Iniciamos análisis de datos.\n');
CapturaID = mopen(Archivo_de_datos, 'r');
if CapturaID == -1 then
    error("No se pudo abrir el archivo.");
else
    mprintf('Archivo de datos abierto.\n\n');
end

// Analizamos lineas -----------------------------------------------------------
while (Leer == VERDADERO) do
    Linea = mgetl(CapturaID, 1);
    if length(Linea) == 0 then
        // No leemos mas
        Leer = FALSO;
    else
        [na, e1a, e2a] = analizo_linea(Linea);
        if (na>0 && na<=MUESTRAS) then
            Entrada1(na) = e1a;
            Entrada2(na) = e2a;
        end
        L2=Linea;
    end
end
mclose('all');

// Graficamos ------------------------------------------------------------------
tiempo = (1-DISPARO):1:(MUESTRAS-DISPARO);
tiempo = tiempo / FM * 1e6;
figure();
xgrid();
f = gcf();
f.background = 8;
plot(tiempo,Entrada1, "r");
plot(tiempo,Entrada2, "b");
xlabel('Tiempo [us]');
ylabel('[V]');
title('Captura guardada en '+Archivo_de_datos);
l = legend('Entrada 1','Entrada 2',  [1, 2], "lower_caption");
l.background = 8;
printf("\nGráfico realizado.")


