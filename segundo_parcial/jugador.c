#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <pthread.h>
#include "def.h"
#include "memoria.h"
#include "cola.h"
#include "thread.h"
#include "global.h"

int main(int argc, char *argv[])
{
    int id_memoria_semaforo;
    int id_cola_mensajes;
    int i;
    semaforo *estadoSemaforo = NULL;
    Jugador *datos_thread;
    pthread_t *idHilo;
    pthread_attr_t atributos;

    estadoSemaforo = (semaforo *)creo_memoria(sizeof(semaforo), &id_memoria_semaforo, CLAVE_BASE);

    estadoSemaforo->activoJugadores = 1;

    while (estadoSemaforo->activoEspiral != 1 || estadoSemaforo->activoJugadores != 1)
    {
        printf("MOSQUITOS: ESPERANDO QUE INICIEN AMBOS PROCESOS\n");
        sleep(1);
    }

    id_cola_mensajes = creoIdColaMensajes(CLAVE_BASE);

    idHilo = (pthread_t *)malloc(sizeof(pthread_t) * CANT_JUGADORES);

    pthread_attr_init(&atributos);
    pthread_attr_setdetachstate(&atributos, PTHREAD_CREATE_JOINABLE);

    datos_thread = (Jugador *)malloc(sizeof(Jugador) * CANT_JUGADORES);

    pthread_mutex_init(&mutex, NULL);

    for (i = 0; i < CANT_JUGADORES; i++)
    {
        datos_thread[i].idJugador = i;
        datos_thread[i].idColaMensajes = id_cola_mensajes;

        pthread_create(&idHilo[i], &atributos, &funcionThread, &datos_thread[i]);
    }

    for (i = 0; i < CANT_JUGADORES; i++)
    {
        pthread_join(idHilo[i], NULL);
        printf("Finalizo el jugador %d \n", datos_thread[i].idJugador + 1);
    }

    shmdt((char *)estadoSemaforo);
    shmctl(id_memoria_semaforo, IPC_RMID, (struct shmid_ds *)NULL);
    free(idHilo);
    borrarColaMensajes(id_cola_mensajes);

    return 0;
}
