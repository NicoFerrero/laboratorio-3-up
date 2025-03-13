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
    int nro_jugador;
    int id_cola_mensajes;
    int done = 0;
    int avance = 0;
    char cadena[LARGO_CADENA];
    mensaje msg;

    Jugador *datos_thread = (Jugador *)parametro;
    nro_jugador = datos_thread->idJugador;
    id_cola_mensajes = datos_thread->idColaMensajes;

    srand(time(NULL));

    while (done == 0)
    {
        recibirMensaje(id_cola_mensajes, MSG_JUGADOR + nro_jugador, &msg); /*bloqueate*/

        switch (msg.int_evento)
        {
        case EVT_INI_MOSQUITO:
            pthread_mutex_lock(&mutex);
            avance = avance + numeroAleatorio(3, 6);
            sprintf(cadena, "%d", avance);
            printf("Mosquito %d va a avanzar %d metros\n", nro_jugador + 1, avance);
            enviarMensaje(id_cola_mensajes, MSG_HUMO, MSG_JUGADOR + nro_jugador, EVT_AVZ_MOSQUITO, cadena);
            pthread_mutex_unlock(&mutex);
            usleep(300 * 1000);
            break;
        case EVT_MOSQUITO_PERD:
            printf("Mosquito %d perdio\n", nro_jugador + 1);
            done = 1;
            break;
        case EVT_MOSQUITO_GAN:
            printf("Mosquito %d le gano al humo\n", nro_jugador + 1);
            done = 1;
            break;
        case EVT_SEGUIR:
            printf("Mosquito %d no llego a la meta. Recorrio %d metros\n", nro_jugador + 1, avance);
            pthread_mutex_lock(&mutex);
            avance = avance + numeroAleatorio(3, 6);
            sprintf(cadena, "%d", avance);
            printf("Mosquito %d va a avanzar %d metros\n", nro_jugador + 1, avance);
            enviarMensaje(id_cola_mensajes, MSG_HUMO, MSG_JUGADOR + nro_jugador, EVT_AVZ_MOSQUITO, cadena);
            pthread_mutex_unlock(&mutex);
            usleep(300 * 1000);
            break;
        default:
            printf("Mosquito:%d Evento sin definir %d\n", nro_jugador + 1, msg.int_evento);
            break;
        }
    }
    pthread_exit((void *)0);
}
