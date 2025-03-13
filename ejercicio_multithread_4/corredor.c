#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <string.h>
#include "def.h"
#include "memoria.h"
#include "cola.h"
#include "thread.h"
#include "global.h"

int main(int argc, char *argv[])
{
    int id_memoria_semaforo;
    int id_memoria_corredores;
    int id_cola_mensajes;
    int i;
    semaforo *estadoSemaforo = NULL;
    DatoHilo *datoHilo = NULL;

    pthread_t *idHilo;
    pthread_attr_t atributos;

    estadoSemaforo = (semaforo *)creo_memoria(sizeof(semaforo), &id_memoria_semaforo, CLAVE_BASE);

    while (estadoSemaforo->activoCarrera != 1)
    {
        printf("ESPERANDO QUE INICIE PROCESO CARRERA\n");
        sleep(1);
    }

    srand(time(NULL));

    id_cola_mensajes = creoIdColaMensajes(CLAVE_BASE);

    idHilo = (pthread_t *)malloc(sizeof(pthread_t) * CANT_CORREDORES);

    pthread_attr_init(&atributos);
    pthread_attr_setdetachstate(&atributos, PTHREAD_CREATE_JOINABLE);

    datoHilo = (DatoHilo *)creo_memoria(sizeof(DatoHilo) * CANT_CORREDORES, &id_memoria_corredores, CLAVE_BASE + 1);

    pthread_mutex_init(&mutex, NULL);

    /*Inicializar luchadores*/
    for (i = 0; i < CANT_CORREDORES; i++)
    {
        datoHilo[i].idCorredor = i;
        datoHilo[i].idColaMensajes = id_cola_mensajes;
    }

    /*Lanzar hilos */
    for (i = 0; i < CANT_CORREDORES; i++)
    {
        pthread_create(&idHilo[i], &atributos, &funcionThread, &datoHilo[i]);
    }

    estadoSemaforo->activoCorredor = 1;

    for (i = 0; i < CANT_CORREDORES; i++)
    {
        pthread_join(idHilo[i], NULL);
        printf("Finalizo %d \n", datoHilo[i].idCorredor + 1);
    }

    shmdt((char *)estadoSemaforo);
    shmdt((char *)datoHilo);
    shmctl(id_memoria_semaforo, IPC_RMID, (struct shmid_ds *)NULL);
    shmctl(id_memoria_corredores, IPC_RMID, (struct shmid_ds *)NULL);
    free(idHilo);
    borrarColaMensajes(id_cola_mensajes);

    return 0;
}
