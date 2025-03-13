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
    int pasos = 0;
    char mensajecorredor[LARGO_MENSAJE];
    mensaje msg;

    DatoHilo *datos_thread = (DatoHilo *)parametro;
    id_cola_mensajes = datos_thread->idColaMensajes;

    while (done == 0)
    {
        recibirMensaje(id_cola_mensajes, MSG_CORREDOR + datos_thread->idCorredor, &msg);

        switch (msg.int_evento)
        {
        case EVT_INICIAR:
            pthread_mutex_lock(&mutex);
            pasos = numeroAleatorio(1, 5);
            if (datos_thread->idCorredor == 0)
            {
                printf("Soy la tortuga y di %d pasos\n", pasos);
            }
            else
            {
                printf("Soy el conejo y di %d pasos\n", pasos);
            }
            sprintf(mensajecorredor, "%d", pasos);
            enviarMensaje(id_cola_mensajes, MSG_CARRERA, MSG_CORREDOR + datos_thread->idCorredor, EVT_AVANZAR, mensajecorredor);
            pthread_mutex_unlock(&mutex);
            if (datos_thread->idCorredor == 0)
            {
                usleep(500 * 1000);
            }
            else
            {
                usleep(100 * 1000);
            }
            break;
        case EVT_CORRER:
            pthread_mutex_lock(&mutex);
            pasos = numeroAleatorio(1, 5);
            if (datos_thread->idCorredor == 0)
            {
                printf("Soy la tortuga y di %d pasos\n", pasos);
            }
            else
            {
                printf("Soy el conejo y di %d pasos\n", pasos);
            }
            sprintf(mensajecorredor, "%d", pasos);
            enviarMensaje(id_cola_mensajes, MSG_CARRERA, MSG_CORREDOR + datos_thread->idCorredor, EVT_AVANZAR, mensajecorredor);
            pthread_mutex_unlock(&mutex);
            if (datos_thread->idCorredor == 0)
            {
                usleep(500 * 1000);
            }
            else
            {
                usleep(100 * 1000);
            }
            break;
        case EVT_ESPERAR:
            pthread_mutex_lock(&mutex);
            pthread_mutex_unlock(&mutex);
            usleep(5000 * 1000);
            break;
        case EVT_PERDEDOR:
            pthread_mutex_lock(&mutex);
            if (datos_thread->idCorredor == 0)
            {
                printf("Soy la tortuga y perdi!!\n");
            }
            else
            {
                printf("Soy el conejo y perdi!!\n");
            }
            done = 1;
            pthread_mutex_unlock(&mutex);
            usleep(500 * 1000);
            break;
        case EVT_GANADOR:
            pthread_mutex_lock(&mutex);
            if (datos_thread->idCorredor == 0)
            {
                printf("Soy la tortuga y gane!!\n");
            }
            else
            {
                printf("Soy el conejo y gane!!\n");
            }
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
