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
    int id_memoria_luchadores;
    int id_cola_mensajes;
    int i;
    semaforo *estadoSemaforo = NULL;
    Luchador *luchadores = NULL;

    pthread_t *idHilo;
    pthread_attr_t atributos;

    estadoSemaforo = (semaforo *)creo_memoria(sizeof(semaforo), &id_memoria_semaforo, CLAVE_BASE);

    while (estadoSemaforo->activo != 1)
    {
        printf("ESPERANDO QUE INICIE PROCESO BOLERA\n");
        sleep(1);
    }

    srand(time(NULL));

    id_cola_mensajes = creoIdColaMensajes(CLAVE_BASE);

    idHilo = (pthread_t *)malloc(sizeof(pthread_t) * CANT_LUCHADORES);

    pthread_attr_init(&atributos);
    pthread_attr_setdetachstate(&atributos, PTHREAD_CREATE_JOINABLE);

    luchadores = (Luchador *)creo_memoria(sizeof(Luchador) * CANT_LUCHADORES, &id_memoria_luchadores, CLAVE_BASE + 1);

    pthread_mutex_init(&mutex, NULL);

    /*Inicializar luchadores*/
    for (i = 0; i < CANT_LUCHADORES; i++)
    {
        if (i == 0)
        {
            memset(luchadores[i].nombre, 0x00, sizeof(luchadores[i].nombre));
            sprintf(luchadores[i].nombre, "%s", "Maximo");
        }
        else if (i > 0 && i < 3)
        {
            memset(luchadores[i].nombre, 0x00, sizeof(luchadores[i].nombre));
            sprintf(luchadores[i].nombre, "Gladiador %d", i);
        }
        else
        {
            memset(luchadores[i].nombre, 0x00, sizeof(luchadores[i].nombre));
            sprintf(luchadores[i].nombre, "Luchador gladiador %d", i - 2);
        }
        memset(luchadores[i].arma, 0x00, sizeof(luchadores[i].arma));
        sprintf(luchadores[i].arma, "Arma %d", i + 1);

        luchadores[i].vida = 150;
        luchadores[i].idLuchador = i;
        luchadores[i].idColaMensajes = id_cola_mensajes;
    }

    /*Lanzar hilos */
    for (i = 0; i < CANT_LUCHADORES; i++)
    {
        pthread_create(&idHilo[i], &atributos, &funcionThread, &luchadores[i]);
    }

    for (i = 0; i < CANT_LUCHADORES; i++)
    {
        pthread_join(idHilo[i], NULL);
        printf("Finalizo %s \n", luchadores[i].nombre);
    }

    shmdt((char *)estadoSemaforo);
    shmdt((char *)luchadores);
    shmctl(id_memoria_semaforo, IPC_RMID, (struct shmid_ds *)NULL);
    shmctl(id_memoria_luchadores, IPC_RMID, (struct shmid_ds *)NULL);
    free(idHilo);
    borrarColaMensajes(id_cola_mensajes);

    return 0;
}
