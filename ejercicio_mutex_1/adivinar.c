#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <pthread.h>
#include "def.h"
#include "semaforo.h"
#include "memoria.h"
#include "funciones.h"
#include "cola.h"
#include "thread.h"
#include "global.h"

int main(int argc, char *argv[])
{
    pthread_t *idHilo;
    pthread_attr_t atributos;
    int cantidad;
    int i;
    ThreadData *thread_data;
    int alguienAdivino = 0;
    int numeroAdivinar;

    if (argc != 2)
    {
        printf("Uso: %s <número de hilos>\n", argv[0]);
        return 1;
    }

    srand(time(NULL));

    cantidad = atoi(argv[1]);
    idHilo = (pthread_t *)malloc(sizeof(pthread_t) * cantidad);
    thread_data = (ThreadData *)malloc(sizeof(ThreadData) * cantidad);
    numeroAdivinar = numeroAleatorio(1, 99);
    printf("Número a adivinar: %d\n", numeroAdivinar);

    pthread_mutex_init(&mutex, NULL);
    pthread_attr_init(&atributos);
    pthread_attr_setdetachstate(&atributos, PTHREAD_CREATE_JOINABLE);

    for (i = 0; i < cantidad; i++)
    {
        thread_data[i].id = i + 1;
        thread_data[i].numeroAdivinar = numeroAdivinar;
        thread_data[i].alguienAdivino = &alguienAdivino;
        thread_data[i].intentos = 0;
        if (pthread_create(&idHilo[i], &atributos, funcionThread, &thread_data[i]) != 0)
        {
            perror("No puedo crear thread");
            exit(-1);
        }
    }

    /*Esperar a que todos los hilos terminen*/
    for (i = 0; i < cantidad; i++)
    {
        pthread_join(idHilo[i], NULL);
    }

    /*Mostrar los resultados*/
    if (alguienAdivino != 0)
    {
        printf("El hilo %d ganó el juego\n", alguienAdivino);
        for (i = 0; i < cantidad; i++)
        {
            printf("Hilo %d intentó %d veces\n", thread_data[i].id, thread_data[i].intentos);
        }
    }

    free(idHilo);
    return 0;
}
