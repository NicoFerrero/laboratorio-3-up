#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "def.h"
#include "archivos.h"
#include "semaforo.h"

int main(int argc, char *argv[])
{
    FILE *archivo;
    int contador = 0;
    char nombreArchivo[LARGO_ARCHIVO];
    int idSemaforo;

    idSemaforo = crearSemaforo();
    iniciarSemaforo(idSemaforo, VERDE);
    printf("Vuelo\tCiudad\n");
    do
    {
        archivo = abrirArchivo(NOMBRE_ARCHIVO, "r");
    } while (archivo == NULL);
    cerrarArchivo(archivo);

    while (1)
    {
        esperarSemaforo(idSemaforo);
        archivo = abrirArchivo(NOMBRE_ARCHIVO, "r");
        if (archivo != NULL)
        {
            leerArchivoCompleto(archivo);
            cerrarArchivo(archivo);
            sprintf(nombreArchivo, "lote.%d.dat", contador);
            if (renombrarArchivo(NOMBRE_ARCHIVO, nombreArchivo) != 0)
            {
                exit(0);
            }
            contador = contador + 1;
        }
        levantarSemaforo(idSemaforo);
        usleep(1000 * 500);
    }
    return 0;
}
