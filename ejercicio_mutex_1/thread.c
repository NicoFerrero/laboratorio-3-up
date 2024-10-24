#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "thread.h"
#include "funciones.h"
#include "global.h"

void *funcionThread(void *parametro)
{
    ThreadData *data = (ThreadData *)parametro;
    int guess;
    int numeros[100] = {0};
    int size = 0;

    while (1)
    {
        pthread_mutex_lock(&mutex);

        /*Si alguien ya adivinó, salir del bucle*/
        if (*data->alguienAdivino != 0)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }

        /*Generar un número aleatorio y contar los intentos*/
        guess = numeroAleatorioNoRepetitivo(1, 99, numeros, &size);
        data->intentos++;
        printf("Hilo %d intenta con el número %d\n", data->id, guess);

        /*Si adivina el número*/
        if (guess == data->numeroAdivinar)
        {
            *data->alguienAdivino = data->id;
        }

        pthread_mutex_unlock(&mutex);
        usleep(1000 * numeroAleatorio(500, 5000));
    }
    pthread_exit((void *)0);
}
