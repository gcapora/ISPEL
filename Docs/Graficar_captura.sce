/*******************************************************************************
* ISPEL: Gráfica de los datos capturados
********************************************************************************
* Se interpreta un archivo de texto con los datos envados por el sistema 
* embebido y se grafica.
*
* UNDAV, marzo de 2024.
*******************************************************************************/

// Variables locales
VERDADERO = %t;
FALSO = %f;
LF = ascii(10);
Leer = %t;
FinLinea = %f;
N = 1;


// Abrimos archivo de datos
CapturaID = mopen('/home/guille/Documentos/CESE/TF/SW/ISPEL/Docs/Captura_N01_24-07-10.txt', 'r');
if CapturaID == -1 then
    error("No se pudo abrir el archivo.");
else
    disp('Archivo de datos abierto.');
end

// Analizamos lineas
while (Leer == VERDADERO) do
    Car = mgetstr(1,CapturaID);
    if Car == "" then
        // Fin del archivo
        Leer = FALSO;
    else if (Car == LF) then
        // Fin de linea --> 
        mprintf(LF);
        N = 1;
    else if (N<10) then
        mprintf(Car);
        N = N+1;
    end
    end
    end
    //min(10, length(Linea)) ));
    //disp(line); // Mostrar la línea leída
end
