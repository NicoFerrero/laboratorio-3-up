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
    int ataque;
    char mensajeLuchador[LARGO_MENSAJE];
    mensaje msg;

    Luchador *datos_thread = (Luchador *)parametro;
    id_cola_mensajes = datos_thread->idColaMensajes;

    while (done == 0)
    {
        recibirMensaje(id_cola_mensajes, MSG_LUCHADOR + datos_thread->idLuchador, &msg);

        switch (msg.int_evento)
        {
        case EVT_TURNO:
            pthread_mutex_lock(&mutex);
            if (datos_thread->idLuchador == 0)
            {
                ataque = numeroAleatorio(5, 10);
                printf("Soy %s y voy a hacer %d de danio\n", datos_thread->nombre, ataque);
            }
            else if (datos_thread->idLuchador > 0 && datos_thread->idLuchador < 3)
            {
                ataque = numeroAleatorio(1, 4);
                printf("Soy el %s y voy a hacer %d de danio\n", datos_thread->nombre, ataque);
            }
            else
            {
                ataque = numeroAleatorio(1, 4);
                printf("Soy el %s y voy a hacer %d de danio\n", datos_thread->nombre, ataque);
            }
            sprintf(mensajeLuchador, "%d", ataque);
            enviarMensaje(id_cola_mensajes, MSG_COLISEO, MSG_LUCHADOR + datos_thread->idLuchador, EVT_ATAQUE, mensajeLuchador);
            pthread_mutex_unlock(&mutex);
            usleep(500 * 1000);
            break;
        case EVT_MUERTO:
            pthread_mutex_lock(&mutex);
            done = 1;
            pthread_mutex_unlock(&mutex);
            usleep(500 * 1000);
            break;
        case EVT_GANADOR:
            pthread_mutex_lock(&mutex);
            done = 1;
            pthread_mutex_unlock(&mutex);
            usleep(500 * 1000);
            break;
        default:
            printf("EVENTO NO MAPEADO %d", msg.int_evento);
            break;
        }
    };
    pthread_exit((void *)0);
}
