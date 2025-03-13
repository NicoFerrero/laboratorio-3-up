#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <pthread.h>
#include "def.h"
#include "memoria.h"
#include "cola.h"
#include "global.h"
#include "funciones.h"

int main(int argc, char *argv[])
{
    int id_memoria_semaforo;
    int id_cola_mensajes;
    int i;
    mensaje msg;
    semaforo *estadoSemaforo = NULL;
    int corredores[CANT_CORREDORES];
    int ganador = 0;
    int distancia = 0;
    int conejoEspera = 0;

    for (i = 0; i < CANT_CORREDORES; i++)
    {
        corredores[i] = 0;
    }

    estadoSemaforo = (semaforo *)creo_memoria(sizeof(semaforo), &id_memoria_semaforo, CLAVE_BASE);

    estadoSemaforo->activoCarrera = 1;

    while (estadoSemaforo->activoCorredor != 1)
    {
        printf("ESPERANDO QUE INICIE PROCESO CORREDOR\n");
        sleep(1);
    }

    id_cola_mensajes = creoIdColaMensajes(CLAVE_BASE);

    srand(time(NULL));

    borrarMensajes(id_cola_mensajes); /*Borra todos los mensajes que haya en la cola.*/

    for (i = 0; i < CANT_CORREDORES; i++)
    {
        enviarMensaje(id_cola_mensajes, MSG_CORREDOR + i, MSG_CARRERA, EVT_CORRER, "");
    }

    while (ganador == 0)
    {
        recibirMensaje(id_cola_mensajes, MSG_CARRERA, &msg);
        switch (msg.int_evento)
        {
        case EVT_AVANZAR:
            distancia = atoi(msg.char_mensaje);
            corredores[msg.int_rte - MSG_CORREDOR] = corredores[msg.int_rte - MSG_CORREDOR] + distancia;
            if (msg.int_rte - MSG_CORREDOR == 0)
            {
                printf("Soy la tortuga y avance %d pasos. En total llevo %d pasos\n", distancia, corredores[msg.int_rte - MSG_CORREDOR]);
            }
            else
            {
                printf("Soy el conejo y avance %d pasos. En total llevo %d pasos\n", distancia, corredores[msg.int_rte - MSG_CORREDOR]);
            }
            if (corredores[msg.int_rte - MSG_CORREDOR] >= 50 && msg.int_rte - MSG_CORREDOR == 1 && conejoEspera == 0)
            {
                enviarMensaje(id_cola_mensajes, msg.int_rte, MSG_CARRERA, EVT_ESPERAR, "");
                printf("Soy el conejo y pase los 50 pasos. Voy a descansar 5 segundos\n");
                conejoEspera = 1;
            }
            if (corredores[msg.int_rte - MSG_CORREDOR] < 100)
            {
                enviarMensaje(id_cola_mensajes, msg.int_rte, MSG_CARRERA, EVT_CORRER, "");
            }
            else
            {
                if (msg.int_rte - MSG_CORREDOR == 0)
                {
                    enviarMensaje(id_cola_mensajes, MSG_CORREDOR, MSG_CARRERA, EVT_GANADOR, "");
                    enviarMensaje(id_cola_mensajes, MSG_CORREDOR + 1, MSG_CARRERA, EVT_PERDEDOR, "");
                    printf("Soy la toruga y gane!!\n");
                }
                else
                {
                    enviarMensaje(id_cola_mensajes, MSG_CORREDOR + 1, MSG_CARRERA, EVT_GANADOR, "");
                    enviarMensaje(id_cola_mensajes, MSG_CORREDOR, MSG_CARRERA, EVT_PERDEDOR, "");
                    printf("Soy el conejo y gane!!\n");
                }
                ganador = 1;
            }
            break;
        default:
            break;
        }
    }

    for (i = 0; i < CANT_CORREDORES; i++)
    {
        printf("Corredor dio %d pasos\n", corredores[i]);

        /* if (corredores[i] >= 100 && i == 0)
        // {
        //     printf("La toruga gano la carrera con un total de %d pasos\n", );
        //     printf("El conejo perdio la carrera con un total de %d pasos\n");
        // }
        // else
        // {
        //     printf("La toruga perdio la carrera con un total de %d pasos\n");
        //     printf("El conejo gano la carrera con un total de %d pasos\n");
        // }*/
    }

    shmdt((char *)estadoSemaforo);
    shmctl(id_memoria_semaforo, IPC_RMID, (struct shmid_ds *)NULL);

    return 0;
}
