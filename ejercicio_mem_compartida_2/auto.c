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
    int idSemaforo, i, vias, viasMenosAutos = -1, cantidadAutos = 0;

    if (argc != 2)
    {
        printf("No ingreso la cantidad de parametros requerida. Debe pasar la cantidad de vias\n");
        return 0;
    }
    vias = atoi(argv[1]);

    signal(SIGINT, handleSiginit);

    srand(time(NULL));

    idSemaforo = crearSemaforo();

    /*EL sizeof(dato) * CANTIDAD es porque quiero un array de estructura dato*/
    /*Como usamos misma clave y comando para crear clave vamos a obtener el mismo segmento de memoria*/
    memoria = (dato *)creo_memoria(sizeof(dato) * vias, &id_memoria, CLAVE_BASE);
    estadoSemaforo = (semaforo *)creo_memoria(sizeof(semaforo), &id_memoria_semaforo, 34);

    while (estadoSemaforo->activo != 1)
    {
        printf("ESPERANDO QUE INICIE PROCESO PEAJE\n");
        sleep(1);
    }

    while (!stop)
    {
        esperarSemaforo(idSemaforo);
        for (i = 0; i < vias; i++)
        {
            if (viasMenosAutos == -1)
            {
                cantidadAutos = memoria[i].vehiculos;
                viasMenosAutos = i;
                continue;
            }
            if (cantidadAutos > memoria[i].vehiculos)
            {
                cantidadAutos = memoria[i].vehiculos;
                viasMenosAutos = i;
            }
        }
        printf("LA VIA SELECCIONADA ES LA %d\n", viasMenosAutos + 1);
        memoria[viasMenosAutos].vehiculos = memoria[viasMenosAutos].vehiculos + 1;
        viasMenosAutos = -1;
        cantidadAutos = 0;
        levantarSemaforo(idSemaforo);
        usleep(1000 * numeroAleatorio(400, 600));
    }
    /*DEBEMOS ASEGURARNOS QUE ESTO EN ALGUN MOMENTO SE VA A EJECUTAR, CON EL WHILE 1 NUCNA LO HARA*/
    shmdt((char *)memoria);
    shmctl(id_memoria, IPC_RMID, (struct shmid_ds *)NULL);
    shmdt((char *)estadoSemaforo);
    shmctl(id_memoria_semaforo, IPC_RMID, (struct shmid_ds *)NULL);
    return 0;
}
