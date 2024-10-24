#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "def.h"
#include "archivos.h"
#include "semaforo.h"

int main(int argc, char *argv[])
{
    /*Si usamos cadenas hay que limpiarla con memset(cadena, 0x00, sizeof(cadena));*/
    FILE *archivo;
    int idSemaforo;
    int numeroVuelo;
    char ciudad[LARGO_CIUDAD];
    char cadena[LARGO_CADENA];

    idSemaforo = crearSemaforo();

    while (1)
    {
        /* PIDO DATOS DEL VUELO */
        printf("ESPERANDO SEMAFORO\n");
        esperarSemaforo(idSemaforo);
        printf("Ingrese el numero de vuelo: ");
        scanf("%d", &numeroVuelo);
        if (numeroVuelo == 0)
        {
            printf("LEVANTO SEMAFORO\n");
            levantarSemaforo(idSemaforo);
            break;
        }
        printf("Ingrese nombre de la ciudad hacia donde vuela (maximo %d caracteres): ", LARGO_CIUDAD);
        memset(ciudad, 0x00, sizeof(ciudad));
        scanf("%49s", ciudad);
        /* Armo la linea a escribir en el archivo */
        memset(cadena, 0x00, sizeof(cadena));
        snprintf(cadena, sizeof(cadena), "%04d\t%s\n", numeroVuelo, ciudad);
        archivo = abrirArchivo("lote.dat", "a");
        if (archivo != NULL)
        {
            escribir(archivo, cadena);
            cerrarArchivo(archivo);
        }
        else
        {
            continue;
        }
        printf("LEVANTO SEMAFORO\n");
        levantarSemaforo(idSemaforo);
        usleep(1000 * 1000);
    }
    return 0;
}
