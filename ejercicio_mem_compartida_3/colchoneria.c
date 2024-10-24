#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include "def.h"
#include "semaforo.h"
#include "memoria.h"
#include "funciones.h"

volatile sig_atomic_t stop;

void handleSiginit(int sig)
{
    stop = 1;
}

int main(int argc, char *argv[])
{
    int id_memoria;
    int id_memoria_semaforo;
    dato *memoria = NULL;
    semaforo *estadoSemaforo = NULL;
    int idSemaforo, i;

    signal(SIGINT, handleSiginit);

    srand(time(NULL));

    idSemaforo = crearSemaforo();

    /*EL sizeof(dato) * CANTIDAD es porque quiero un array de estructura dato*/
    /*Como usamos misma clave y comando para crear clave vamos a obtener el mismo segmento de memoria*/
    memoria = (dato *)creo_memoria(sizeof(dato) * CANTIDAD, &id_memoria, CLAVE_BASE);
    estadoSemaforo = (semaforo *)creo_memoria(sizeof(semaforo), &id_memoria_semaforo, 34);

    for (i = 0; i < CANTIDAD; i++)
    {
        memoria[i].cantidad = 10;
        memoria[i].codigo = i + 1;
        memset(memoria[i].nombre, 0x00, sizeof(memoria[i].nombre));
        switch (i)
        {
        case 0:
            memcpy(memoria[i].nombre, "PIERO", strlen("PIERO") + 1);
            break;
        case 1:
            memcpy(memoria[i].nombre, "SUAVESTAR", strlen("SUAVESTAR") + 1);
            break;
        case 2:
            memcpy(memoria[i].nombre, "CANNON", strlen("CANNON") + 1);
            break;
        case 3:
            memcpy(memoria[i].nombre, "SIMMONS", strlen("SIMMONS") + 1);
            break;
        case 4:
            memcpy(memoria[i].nombre, "BELMO", strlen("BELMO") + 1);
            break;

        default:
            break;
        }
    }

    estadoSemaforo->activo = 1;

    while (!stop)
    {
        esperarSemaforo(idSemaforo);
        printf("\n******ESTADO DE STOCK******");
        printf("\nCODIGO\tNOMBRE\tCANTIDAD\n");
        for (i = 0; i < CANTIDAD; i++)
        {
            printf("%d\t%s\t%d\n", memoria[i].codigo, memoria[i].nombre, memoria[i].cantidad);
            if (memoria[i].cantidad < 10)
            {
                memoria[i].cantidad = memoria[i].cantidad + 1;
            }
        }
        levantarSemaforo(idSemaforo);
        usleep(1000 * 500);
    }
    /*DEBEMOS ASEGURARNOS QUE ESTO EN ALGUN MOMENTO SE VA A EJECUTAR, CON EL WHILE 1 NUCNA LO HARA*/
    shmdt((char *)memoria);
    shmctl(id_memoria, IPC_RMID, (struct shmid_ds *)NULL);
    shmdt((char *)estadoSemaforo);
    shmctl(id_memoria_semaforo, IPC_RMID, (struct shmid_ds *)NULL);
    return 0;
}
