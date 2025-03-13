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
    int idJugador;
    int id_cola_mensajes;
    int done = 0;
    int tiro;
    int puntaje;
    mensaje msg;

    Jugador *datos_thread = (Jugador *)parametro;
    idJugador = datos_thread->idJugador;
    id_cola_mensajes = datos_thread->idColaMensajes;

    while (done == 0)
    {
        recibirMensaje(id_cola_mensajes, MSG_JUGADOR + idJugador, &msg);

        switch (msg.int_evento)
        {
        case EVT_INICIO:
            pthread_mutex_lock(&mutex);
            printf("Soy el jugador %d y voy a hacer mi primer tiro\n", MSG_JUGADOR + idJugador - 1);
            enviarMensaje(id_cola_mensajes, MSG_BOLERA, MSG_JUGADOR + idJugador, EVT_TIRO, "");
            pthread_mutex_unlock(&mutex);
            usleep(500 * 1000);
            break;
        case EVT_RESULTADO:
            pthread_mutex_lock(&mutex);
            tiro = atoi(msg.char_mensaje);
            printf("Soy el jugador %d y tire %d bolos\n", MSG_JUGADOR + idJugador - 1, tiro);
            enviarMensaje(id_cola_mensajes, MSG_BOLERA, MSG_JUGADOR + idJugador, EVT_TIRO, "");
            pthread_mutex_unlock(&mutex);
            usleep(500 * 1000);
            break;
        case EVT_FIN:
            pthread_mutex_lock(&mutex);
            puntaje = atoi(msg.char_mensaje);
            printf("Soy el jugador %d y termine el juego con %d puntos\n", MSG_JUGADOR + idJugador - 1, puntaje);
            pthread_mutex_unlock(&mutex);
            usleep(500 * 1000);
            done = 1;
            break;
        default:
            printf("EVENTO NO MAPEADO %d", msg.int_evento);
            break;
        }
    };
    pthread_exit((void *)0);
}
