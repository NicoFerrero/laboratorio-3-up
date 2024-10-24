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
    char *lote;
    int idSemaforo;
    int panel;
    char mensaje[LARGO_CADENA];

    if (argc != 2)
    {
        printf("No ingreso la cantidad de parametros requerida. Debe pasar la cantidad de lotes\n");
    }
    panel = atoi(argv[1]);
    if (panel != 1 && panel != 2 && panel != 3)
    {
        printf("Los paneles disponibles son el 1, 2 o 3\n");
        return 0;
    }

    switch (panel)
    {
    case 1:
        lote = LOTE_1;
        break;
    case 2:
        lote = LOTE_2;
        break;
    case 3:
        lote = LOTE_3;
        break;
    default:
        break;
    }
    idSemaforo = crearSemaforo();
    esperarSemaforo(idSemaforo);
    archivo = abrirArchivo(lote, "r");
    while (archivo == NULL)
    {
        archivo = abrirArchivo(lote, "w");
        printf("EL PANEL NO TIENE UN MENSAJE. GRABANDO MENSAJE DEFAULT...\n");
        escribir(archivo, "default");
        cerrarArchivo(archivo);
    }
    levantarSemaforo(idSemaforo);

    while (1)
    {
        esperarSemaforo(idSemaforo);
        archivo = abrirArchivo(lote, "r");
        if (archivo != NULL)
        {
            system("clear");
            printf("PANEL %d\n", panel);
            while (leerArchivoCompleto(archivo, "%[^\n]", mensaje))
            {
                printf("MENSAJE: %s\n", mensaje);
            }
            cerrarArchivo(archivo);
        }
        levantarSemaforo(idSemaforo);
        usleep(1000 * 100);
    }
    return 0;
}
