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
    int id_memoria_pateadores;
    int id_cola_mensajes;
    int i;
    char tiro[LARGO_MENSAJE] = "";

    semaforo *estadoSemaforo = NULL;
    Pateador *pateadores = NULL;

    pthread_t *idHilo;
    pthread_attr_t atributos;

    estadoSemaforo = (semaforo *)creo_memoria(sizeof(semaforo), &id_memoria_semaforo, CLAVE_BASE);
    estadoSemaforo->activoPateador = 1;

    while (estadoSemaforo->activoArquero != 1)
    {
        printf("ESPERANDO QUE INICIE PROCESO ARQUERO\n");
        sleep(1);
    }

    srand(time(NULL));

    id_cola_mensajes = creoIdColaMensajes(CLAVE_BASE);

    idHilo = (pthread_t *)malloc(sizeof(pthread_t) * CANT_PATEADORES);

    pthread_attr_init(&atributos);
    pthread_attr_setdetachstate(&atributos, PTHREAD_CREATE_JOINABLE);

    pateadores = (Pateador *)creo_memoria(sizeof(Pateador) * CANT_PATEADORES, &id_memoria_pateadores, CLAVE_BASE + 1);

    pthread_mutex_init(&mutex, NULL);

    /*Inicializar luchadores*/
    for (i = 0; i < CANT_PATEADORES; i++)
    {
        pateadores[i].idPateador = i;
        pateadores[i].idColaMensajes = id_cola_mensajes;
        pateadores[i].eventoTiro = 0;
        printf("Ingrese el nombre del pateador %d: ", i + 1);
        memset(pateadores[i].nombre, 0x00, sizeof(pateadores[i].nombre));
        scanf(" %[^\n]", pateadores[i].nombre);
    }

    /*Lanzar hilos */
    for (i = 0; i < CANT_PATEADORES; i++)
    {
        pthread_create(&idHilo[i], &atributos, &funcionThread, &pateadores[i]);
    }

    for (i = 0; i < CANT_PATEADORES; i++)
    {
        pthread_join(idHilo[i], NULL);
        printf("Finalizo %d \n", pateadores[i].idPateador + 1);
    }

    printf("NOMBRE\t\tRESULTADO_TIRO\n");
    for (i = 0; i < CANT_PATEADORES; i++)
    {
        memset(tiro, 0x00, sizeof(tiro));
        switch (pateadores[i].eventoTiro)
        {
        case 2:
            sprintf(tiro, "%s", "PALO");
            break;
        case 3:
            sprintf(tiro, "%s", "TRAVESANO");
            break;
        case 4:
            sprintf(tiro, "%s", "GOL");
            break;
        case 5:
            sprintf(tiro, "%s", "FUERA");
            break;

        default:
            break;
        }
        printf("%s\t\t%s\n", pateadores[i].nombre, tiro);
    }

    shmdt((char *)estadoSemaforo);
    shmdt((char *)pateadores);
    shmctl(id_memoria_semaforo, IPC_RMID, (struct shmid_ds *)NULL);
    shmctl(id_memoria_pateadores, IPC_RMID, (struct shmid_ds *)NULL);
    free(idHilo);
    borrarColaMensajes(id_cola_mensajes);

    return 0;
}
