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
    int cant_jugadores = 3;
    semaforo *estadoSemaforo = NULL;
    int ganador = 0;

    Jugador *datos_thread;
    pthread_t *idHilo;
    pthread_attr_t atributos;

    estadoSemaforo = (semaforo *)creo_memoria(sizeof(semaforo), &id_memoria_semaforo, CLAVE_BASE);

    while (estadoSemaforo->activo != 1)
    {
        printf("ESPERANDO QUE INICIE PROCESO TABLERO\n");
        sleep(1);
    }

    id_cola_mensajes = creoIdColaMensajes(CLAVE_BASE);

    idHilo = (pthread_t *)malloc(sizeof(pthread_t) * cant_jugadores);

    pthread_attr_init(&atributos);
    pthread_attr_setdetachstate(&atributos, PTHREAD_CREATE_JOINABLE);

    datos_thread = (Jugador *)malloc(sizeof(Jugador) * cant_jugadores);

    pthread_mutex_init(&mutex, NULL);

    for (i = 0; i < cant_jugadores; i++)
    {
        datos_thread[i].tipo = CONEJO + i;
        datos_thread[i].idColaMensajes = id_cola_mensajes;
        datos_thread[i].pasos = 0;
        datos_thread[i].turnos = 0;
        datos_thread[i].ganador = &ganador;

        pthread_create(&idHilo[i], &atributos, funcionThread, &datos_thread[i]);
    }

    for (i = 0; i < cant_jugadores; i++)
    {
        pthread_join(idHilo[i], NULL);
        printf("Finalizo el jugador %d \n", datos_thread[i].tipo);
    }

    shmdt((char *)estadoSemaforo);
    shmctl(id_memoria_semaforo, IPC_RMID, (struct shmid_ds *)NULL);
    free(idHilo);
    borrarColaMensajes(id_cola_mensajes);

    return 0;
}
