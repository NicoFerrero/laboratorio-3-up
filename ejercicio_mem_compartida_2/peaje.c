#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <signal.h>
#include <sys/shm.h>
#include "def.h"
#include "semaforo.h"
#include "memoria.h"
#include "funciones.h"
#include "archivos.h"

volatile sig_atomic_t stop;

void handleSiginit(int sig)
{
    stop = 1;
}

int main(int argc, char *argv[])
{
    FILE *archivo;
    int id_memoria;
    int id_memoria_semaforo;
    dato *memoria = NULL;
    semaforo *estadoSemaforo = NULL;
    int idSemaforo, i, vias, liberaciones = 0;
    char texto[LARGO_CIUDAD];

    if (argc != 2)
    {
        printf("No ingreso la cantidad de parametros requerida. Debe pasar la cantidad de vias\n");
        return 0;
    }
    vias = atoi(argv[1]);

    signal(SIGINT, handleSiginit);

    srand(time(NULL));

    idSemaforo = crearSemaforo();
    iniciarSemaforo(idSemaforo, VERDE);

    /*EL sizeof(dato) * CANTIDAD es porque quiero un array de estructura dato*/
    /*Como usamos misma clave y comando para crear clave vamos a obtener el mismo segmento de memoria*/
    memoria = (dato *)creo_memoria(sizeof(dato) * vias, &id_memoria, CLAVE_BASE);
    estadoSemaforo = (semaforo *)creo_memoria(sizeof(semaforo), &id_memoria_semaforo, 34);

    for (i = 0; i < vias; i++)
    {
        memoria[i].vehiculos = numeroAleatorio(1, 7);
    }

    estadoSemaforo->activo = 1;

    while (!stop)
    {
        esperarSemaforo(idSemaforo);
        printf("\n******ESTADO DE VIAS******\n");
        for (i = 0; i < vias; i++)
        {
            printf("VIA[%d]\t%d VEHICULOS\n", i + 1, memoria[i].vehiculos);
            if (memoria[i].vehiculos > 10)
            {
                printf("LIBREANDO VIA %d\n", i + 1);
                memoria[i].vehiculos = 0;
                liberaciones = liberaciones + 1;
                archivo = abrirArchivo(NOMBRE_ARCHIVO, "w");
                if (archivo != NULL)
                {
                    memset(texto, 0x00, sizeof(texto));
                    snprintf(texto, sizeof(texto), "%d", liberaciones);
                    escribir(archivo, texto);
                    cerrarArchivo(archivo);
                }
            }
            else if (memoria[i].vehiculos > 0 && memoria[i].vehiculos <= 10)
            {
                memoria[i].vehiculos = memoria[i].vehiculos - 1;
            }
        }
        levantarSemaforo(idSemaforo);
        usleep(10000 * numeroAleatorio(100, 5000));
    }
    /*DEBEMOS ASEGURARNOS QUE ESTO EN ALGUN MOMENTO SE VA A EJECUTAR, CON EL WHILE 1 NUCNA LO HARA*/
    shmdt((char *)memoria);
    shmctl(id_memoria, IPC_RMID, (struct shmid_ds *)NULL);
    shmdt((char *)estadoSemaforo);
    shmctl(id_memoria_semaforo, IPC_RMID, (struct shmid_ds *)NULL);
    return 0;
}
