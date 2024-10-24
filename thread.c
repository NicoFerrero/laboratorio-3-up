#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "thread.h"
#include "funciones.h"
#include "global.h"

void *funcionThread(void *parametro)
{
    Jugador *jugador = (Jugador *)parametro;
    int numeros[100] = {0};
    int size = 0;
    int i;
    int bolillaActual;

    if (jugador->initCarton == 0)
    {
        for (i = 0; i < 5; i++)
        {
            jugador->carton[i] = numeroAleatorioNoRepetitivo(1, 99, numeros, &size);
        }
        jugador->initCarton = 1;
    }

    while (*jugador->cartonLLeno == 0)
    {
        pthread_mutex_lock(&mutex);
        /*Esperar hasta que se saque una nueva bolilla que el jugador aún no haya verificado*/
        while (*jugador->totalBolillas == jugador->bolillasVerificadas && *jugador->cartonLLeno == 0)
        {
            /*printf("%d ESPERO NUEVA BOLILLA\n", jugador->id);*/
            pthread_cond_wait(&cond, &mutex);
        }

        if (*jugador->cartonLLeno != 0)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }

        bolillaActual = jugador->bolillas[*jugador->totalBolillas - 1];
        /*printf("%d VERIFICANDO BOLILLAS\n", jugador->id);*/
        for (i = 0; i < 5; i++)
        {
            if (jugador->carton[i] == bolillaActual)
            {
                jugador->aciertos++;
                printf("Jugador %d marcó el número %d (Aciertos: %d)\n", jugador->id, bolillaActual, jugador->aciertos);
                break;
            }
        }

        /*Si el jugador completó su cartón*/
        if (jugador->aciertos == 5)
        {
            *jugador->cartonLLeno = jugador->id;
            /*Despertar a los hilos ya que hay un ganador*/
            pthread_cond_broadcast(&cond);
        }

        /*Actualizar la cantidad de bolillas que ha verificado el jugador*/
        jugador->bolillasVerificadas = *jugador->totalBolillas;
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit((void *)0);
}
