/*******************************************************************************
* ISPEL: Lee el archivo de texto caracter a caracter
********************************************************************************
* Lee caracter a caracter el archivo de texto 
* con los datos envados por el equipo.
*
* Mayo de 2025.
*******************************************************************************/

// Variables locales
VERDADERO = %t;
FALSO = %f;
LF = ascii(10);
CR = ascii(13);
TAB = ascii(9);
Leer = VERDADERO;
FinLinea = FALSO;
NC = 1;
NL = 0;


// Abrimos archivo de datos
CapturaID = mopen('Captura_N02_24-07-10.txt', 'r');
if CapturaID == -1 then
    error("No se pudo abrir el archivo.");
else
    printf("Abrimos archivo de datos.\n\n");
    Leer = VERDADERO;
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
        NC = 1;
        NL=NL + 1;
    else if (Car == CR) then 
        // No hago nada!!! 
        Car = CR;
    else if (Car == TAB) then 
        // Tabulación...
        mprintf("\t");
    else if (NC<80) then
        mprintf(Car);
        NC = NC+1;
    end
    end
    end
    end
    end
    //min(10, length(Linea)) ));
    //disp(line); // Mostrar la línea leída
end
printf("\nLeímos %d líneas.", NL);
mclose('all');
