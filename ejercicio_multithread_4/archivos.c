#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "archivos.h"
#include "def.h"

FILE *abrirArchivo(char archivo[], char modo[])
{
    FILE *fp;
    fp = fopen(archivo, modo);
    return fp;
}

void cerrarArchivo(FILE *archivo)
{
    fclose(archivo);
}

void escribir(FILE *archivo, char linea[])
{
    if (fprintf(archivo, "%s", linea) < 0)
    {
        exit(0);
    }
}

int leerArchivoCompleto(FILE *archivo, char *formato, ...)
{
    va_list args;
    int resultado;
    va_start(args, formato);
    resultado = vfscanf(archivo, formato, args);
    va_end(args);

    if (resultado == EOF)
    {
        return 0;
    }
    return 1;
}

/*
NO SE PUEDE HACER EL PRINTF DENTRO DEBE DEVOLVER LINHA Y EL PRINTF VA FUERA
void leerLineaArchivo(FILE *archivo, int numeroLinea)
{
    int numeroVuelo;
    int contadorLinea = 1;
    char ciudad[LARGO_CADENA];

    while (fscanf(archivo, "%d - %s", &numeroVuelo, ciudad) != EOF)
    {
        if (contadorLinea == numeroLinea)
        {
            printf("Linea %d: %d - %s\n", contadorLinea, numeroVuelo, ciudad);
            break;
        }
        contadorLinea++;
    }

    if (contadorLinea < numeroLinea)
    {
        printf("ERROR: LA LINEA %d NO EXISTE EN EL ARCHIVO\n", numeroLinea);
    }
}

NO SE PUEDE HACER EL PRINTF DENTRO DEBE DEVOLVER LINHA Y EL PRINTF VA FUERA
void leerArchivoDesdeHasta(FILE *archivo, int lineaInicio, int lineaFin)
{
    int numeroVuelo;
    int contadorLinea = 1;
    int lineasValidas = 0;
    char ciudad[LARGO_CADENA];

    if (lineaInicio > lineaFin)
    {
        printf("ERROR: EL NUMERO DE LA LNEA DE INICIO ES MAYOR QUE LA LINEA FINAL\n");
        return;
    }

    while (fscanf(archivo, "%d - %s", &numeroVuelo, ciudad) != EOF)
    {
        if (contadorLinea >= lineaInicio && contadorLinea <= lineaFin)
        {
            printf("Linea %d: %d - %s\n", contadorLinea, numeroVuelo, ciudad);
            lineasValidas = 1;
        }
        contadorLinea++;
    }

    if (!lineasValidas)
    {
        printf("ERROR: LAS LINEAS NO EXISTEN EN EL ARCHIVO\n");
    }
}
*/

void leer(FILE *archivo)
{
    char buffer[LARGO_CADENA];

    while (fgets(buffer, sizeof(buffer), archivo) != NULL)
    {
        printf("%s", buffer);
    }
}

int renombrarArchivo(char viejoNombre[], char nuevoNombre[])
{
    return rename(viejoNombre, nuevoNombre);
}
