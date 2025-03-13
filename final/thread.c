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
    int metros = 0;
    char mensajecorredor[LARGO_MENSAJE];
    mensaje msg;

    Corredor *corredor = (Corredor *)parametro;
    id_cola_mensajes = corredor->idColaMensajes;

    while (done == 0)
    {
        recibirMensaje(id_cola_mensajes, MSG_CORREDOR + corredor->idCorredor, &msg);

        switch (msg.int_evento)
        {
        case EVT_CORRER:
            pthread_mutex_lock(&mutex);
            if (*corredor->ganador == 1)
            {
                done = 1;
            }
            else
            {
                if (corredor->esMasRapido == 1)
                {
                    metros = numeroAleatorio(6, 12);
                }
                else
                {
                    metros = numeroAleatorio(5, 10);
                }
                printf("Soy %s y di %d metros\n", corredor->nombre, metros);
                sprintf(mensajecorredor, "%d", metros);
                enviarMensaje(id_cola_mensajes, MSG_CARRERA, MSG_CORREDOR + corredor->idCorredor, EVT_AVANZAR, mensajecorredor);
            }
            pthread_mutex_unlock(&mutex);
            usleep(500 * 1000);
            break;
        case EVT_BOXES:
            pthread_mutex_lock(&mutex);
            if (*corredor->ganador == 1)
            {
                done = 1;
            }
            else
            {
                metros = 0;
                printf("Soy %s y entre a boxes. No avanzo este turno\n", corredor->nombre);
                sprintf(mensajecorredor, "%d", metros);
                enviarMensaje(id_cola_mensajes, MSG_CARRERA, MSG_CORREDOR + corredor->idCorredor, EVT_AVANZAR, mensajecorredor);
            }
            pthread_mutex_unlock(&mutex);
            usleep(500 * 1000);
            break;
        case EVT_SALIO_PISTA:
            pthread_mutex_lock(&mutex);
            if (*corredor->ganador == 1)
            {
                done = 1;
            }
            else
            {
                printf("Soy %s y me sali de pista. Perdi la carrera\n", corredor->nombre);
                done = 1;
                corredor->termine = 1;
            }

            pthread_mutex_unlock(&mutex);
            usleep(500 * 1000);
            break;
        case EVT_CHOCO:
            pthread_mutex_lock(&mutex);
            if (*corredor->ganador == 1)
            {
                done = 1;
            }
            else
            {
                printf("Soy %s y choque. Sigo en carrera\n", corredor->nombre);
                if (corredor->esMasRapido == 1)
                {
                    metros = numeroAleatorio(6, 12);
                }
                else
                {
                    metros = numeroAleatorio(5, 10);
                }
                printf("Soy %s y di %d metros\n", corredor->nombre, metros);
                sprintf(mensajecorredor, "%d", metros);
                enviarMensaje(id_cola_mensajes, MSG_CARRERA, MSG_CORREDOR + corredor->idCorredor, EVT_AVANZAR, mensajecorredor);
            }
            pthread_mutex_unlock(&mutex);
            usleep(500 * 1000);
            break;
        case EVT_PERDEDOR:
            pthread_mutex_lock(&mutex);
            printf("Soy %s y perdi la carrera\n", corredor->nombre);
            done = 1;
            corredor->termine = 1;
            pthread_mutex_unlock(&mutex);
            usleep(500 * 1000);
            break;
        case EVT_GANADOR:
            pthread_mutex_lock(&mutex);
            printf("Soy %s y gane la carrera\n", corredor->nombre);
            done = 1;
            corredor->termine = 1;
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
