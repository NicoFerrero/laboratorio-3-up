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
    Corredor *corredores = NULL;

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

    corredores = (Corredor *)creo_memoria(sizeof(Corredor) * CANT_CORREDORES, &id_memoria_corredores, CLAVE_BASE + 1);

    pthread_mutex_init(&mutex, NULL);
    estadoSemaforo->ganador = 0;

    /*Inicializar luchadores*/
    corredores[0].idCorredor = 0;
    corredores[0].idColaMensajes = id_cola_mensajes;
    corredores[0].esMasRapido = 1;
    corredores[0].termine = 0;
    corredores[0].ganador = &estadoSemaforo->ganador;
    memset(corredores[0].nombre, 0x00, sizeof(corredores[0].nombre));
    sprintf(corredores[0].nombre, "%s", "Ferrari");
    corredores[1].idCorredor = 1;
    corredores[1].idColaMensajes = id_cola_mensajes;
    corredores[1].esMasRapido = 1;
    corredores[1].termine = 0;
    corredores[1].ganador = &estadoSemaforo->ganador;
    memset(corredores[1].nombre, 0x00, sizeof(corredores[1].nombre));
    sprintf(corredores[1].nombre, "%s", "Mclaren");

    for (i = 2; i < CANT_CORREDORES; i++)
    {
        corredores[i].idCorredor = i;
        corredores[i].idColaMensajes = id_cola_mensajes;
        corredores[i].esMasRapido = 0;
        corredores[i].termine = 0;
        corredores[i].ganador = &estadoSemaforo->ganador;
        printf("Ingrese el nombre del corredor %d: ", i + 1);
        memset(corredores[i].nombre, 0x00, sizeof(corredores[i].nombre));
        scanf(" %[^\n]", corredores[i].nombre);
    }

    /*Lanzar hilos */
    for (i = 0; i < CANT_CORREDORES; i++)
    {
        pthread_create(&idHilo[i], &atributos, &funcionThread, &corredores[i]);
    }

    estadoSemaforo->activoCorredor = 1;

    for (i = 0; i < CANT_CORREDORES; i++)
    {
        pthread_join(idHilo[i], NULL);
        printf("Finalizo %d \n", corredores[i].idCorredor + 1);
    }

    shmdt((char *)estadoSemaforo);
    shmdt((char *)corredores);
    shmctl(id_memoria_semaforo, IPC_RMID, (struct shmid_ds *)NULL);
    shmctl(id_memoria_corredores, IPC_RMID, (struct shmid_ds *)NULL);
    free(idHilo);
    borrarColaMensajes(id_cola_mensajes);

    return 0;
}
