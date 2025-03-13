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
    int pasos;
    int turno;
    int i;
    char cadena[LARGO_CADENA];
    mensaje msg;
    int turnosGanador;

    Jugador *datos_thread = (Jugador *)parametro;
    nro_jugador = datos_thread->tipo;
    id_cola_mensajes = datos_thread->idColaMensajes;
    turno = datos_thread->turnos;
    pasos = datos_thread->pasos;

    srand(time(NULL));

    while (done == 0)
    {
        if (pasos < 117 && *datos_thread->ganador == 0)
        {
            pthread_mutex_lock(&mutex);
            switch (nro_jugador)
            {
            case CONEJO:
                pasos = pasos + numeroAleatorio(4, 9);
                break;
            case GATO:
                pasos = pasos + numeroAleatorio(1, 10);
                break;
            case PERRO:
                pasos = pasos + numeroAleatorio(3, 8);
                break;

            default:
                pasos = pasos + 0;
                break;
            }
            turno = turno + 1;
            sprintf(cadena, "%d", pasos);
            printf("Jugador %d va a avanzar %d pasos\n", nro_jugador, pasos);
            enviarMensaje(id_cola_mensajes, MSG_PISTA, nro_jugador, EVT_CORRO, cadena);
            pthread_mutex_unlock(&mutex);
            usleep(500 * 1000);
        }
        recibirMensaje(id_cola_mensajes, nro_jugador, &msg); /*bloqueate*/

        switch (msg.int_evento)
        {
        case EVT_GANADOR:
            pthread_mutex_lock(&mutex);
            *datos_thread->ganador = nro_jugador;
            printf("Soy jugador %d y gane con %d pasos en %d turnos\n", nro_jugador, pasos, turno);
            for (i = 0; i < 3; i++)
            {
                sprintf(cadena, "%d", turno);
                enviarMensaje(id_cola_mensajes, MSG_CONEJO + i, MSG_CONEJO + i, EVT_TERMINAR, cadena);
            }
            pthread_mutex_unlock(&mutex);
            usleep(500 * 1000);
            break;
        case EVT_SEGUIR:
            printf("Jugador %d no llego a la meta. Diste %d pasos en %d turnos\n", nro_jugador, pasos, turno);
            break;
        case EVT_TERMINAR:
            turnosGanador = atoi(msg.char_mensaje);
            /* SI HUBIERA UN GANADOR QUE TUVO MAS TIROS QUE OTRO JUGADOR EESTE SIGUE TIRANDO HASTA TENER LA MISMA CANTIDAD
            printf("TERMINAR TURNOS\nGanador jugo %d turnos y yo %d\n", turnosGanador, turno);
            while (turnosGanador > turno)
            {
                turno = turno + 1;
                switch (nro_jugador)
                {
                case CONEJO:
                    pasos = pasos + numeroAleatorio(4, 9);
                    break;
                case GATO:
                    pasos = pasos + numeroAleatorio(1, 10);
                    break;
                case PERRO:
                    pasos = pasos + numeroAleatorio(3, 8);
                    break;

                default:
                    pasos = pasos + 0;
                    break;
                }
                printf("Con mi tiro extra tengo %d pasos\n", pasos);
            }

            */
            pthread_mutex_lock(&mutex);
            printf("Soy jugador %d y termine la carreara con %d pasos en %d turnos\n", nro_jugador, pasos, turno);
            sprintf(cadena, "%d", pasos);
            enviarMensaje(id_cola_mensajes, MSG_PISTA, nro_jugador, EVT_FIN, cadena);
            pthread_mutex_unlock(&mutex);
            usleep(500 * 1000);
            done = 1;
            break;
        default:
            printf("\nJugador:%d Evento sin definir %d\n", nro_jugador, msg.int_evento);
            break;
        }
    };
    pthread_exit((void *)0);
}
