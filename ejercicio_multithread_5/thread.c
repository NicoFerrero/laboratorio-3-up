#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "def.h"
#include "funciones.h"
#include "cola.h"
#include "thread.h"
#include "global.h"

void *funcionThread(void *parametro)
{
    int id_cola_mensajes;
    int done = 0;
    mensaje msg;

    Pateador *datos_thread = (Pateador *)parametro;
    id_cola_mensajes = datos_thread->idColaMensajes;

    pthread_mutex_lock(&mutex);
    usleep(1000 * 500);
    enviarMensaje(id_cola_mensajes, MSG_ARQUERO, MSG_PATEADOR + datos_thread->idPateador, EVT_TIRO, "");
    printf("Soy %s y voy a patear\n", datos_thread->nombre);
    /*pthread_mutex_unlock(&mutex);
    usleep(1000 * 200);*/

    while (done == 0)
    {
        recibirMensaje(id_cola_mensajes, MSG_PATEADOR + datos_thread->idPateador, &msg);
        switch (msg.int_evento)
        {
        case EVT_GOL:
            /*pthread_mutex_lock(&mutex);*/
            printf("GOOOOOL!\n");
            pthread_mutex_unlock(&mutex);
            usleep(1000 * 200);
            break;
        case EVT_TRAVESANO:
        case EVT_PALO:
        case EVT_FUERA:
            /*pthread_mutex_lock(&mutex);*/
            printf("UFFFFF!\n");
            pthread_mutex_unlock(&mutex);
            usleep(1000 * 200);
            break;
        default:
            printf("EVENTO NO MAPEADO %d", msg.int_evento);
            break;
        }
        done = 1;
        datos_thread->eventoTiro = msg.int_evento;
    };
    pthread_exit((void *)0);
}
