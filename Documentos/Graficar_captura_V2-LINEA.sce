/*******************************************************************************
* ISPEL: Gráfica de los datos capturados (formateado en lineas)
********************************************************************************
* Se interpreta un archivo de texto con los datos envados por el sistema 
* embebido y se grafica.
*
* UNDAV, marzo de 2024.
*******************************************************************************/

// Variables locales -----------------------------------------------------------

VERDADERO = %t;
FALSO = %f;
LF = ascii(10);
LeyendoDatos = FALSO;
EsDato = FALSO;
Leer = %t;
Linea="";
L2="";
N = 1;

FM = 2400000;
MUESTRAS = 6000;
DISPARO = 1500;
E1_VMAX = 3.3;
E1_VMIN = 0;
E2_VMAX = 3.3;
E2_VMIN = 0;
Entrada1 = zeros(1,MUESTRAS);
Entrada2 = zeros(1,MUESTRAS);

// Funciones privadas ----------------------------------------------------------

function [num, d1, d2] = analizo_linea(str)
    global LeyendoDatos; 
    global N;
    num = 0;
    d1 = 0;
    d2 = 0;
    // Es un comentario?
    if ((part(str, 1:3)=="// ") || (part(str, 1:4)=="MSJ ")) then
        // Es un comentario...
        mprintf("%s\n", str);
    end
    // Debo analizar dato?
    if (LeyendoDatos == VERDADERO) then
        if (isnum(part(str,1))) then
            separador = strindex(str,char(9));
            largo     = length(str);
            num = N;
            d1 = strtod(part(str,1:(separador-1)))     * (E1_VMAX-E1_VMIN) / 4092 + E1_VMIN;
            d2 = strtod(part(str,(separador+1):largo)) * (E2_VMAX-E2_VMIN) / 4092 + E2_VMIN;
            N = N+1;
        end
    end
    // Es linea preiva a dato?
    if (part(str, 1:6)=="DATOS ") then
        LeyendoDatos = VERDADERO;
        N=1;
        mprintf("Datos...\n");
    end
    
endfunction

// Abrimos archivo de datos ----------------------------------------------------

close();
mprintf('\n================================================================================\n');
mprintf('ISPEL: Iniciamos análisis de datos.\n');
CapturaID = mopen('Captura_N02_24-07-10.txt', 'r');
if CapturaID == -1 then
    error("No se pudo abrir el archivo.");
else
    mprintf('Archivo de datos abierto.\n');
end

// Analizamos lineas

mprintf('================================================================================\n');
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
            /*mprintf("N=%i\t", na );
            mprintf("E1=%f\t",  e1a );
            mprintf("E1=%f\n",  e2a );*/
        end
        L2=Linea;
    end
end
mclose(CapturaID);

// Graficamos ------------------------------------------------------------------
tiempo = (1-DISPARO):1:(MUESTRAS-DISPARO);
tiempo = tiempo / FM * 1e6;
figure();
f = gcf();
f.background = 8;
plot(tiempo,Entrada1, "r");
plot(tiempo,Entrada2, "b");
xlabel('Tiempo [us]');
ylabel('[V]');
title('Captura de señales N02');
l = legend('Entrada 1','Entrada 2',  [1, 2], "lower_caption");
l.background = 8;

