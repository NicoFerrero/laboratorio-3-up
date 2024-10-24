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
    Jugador *jugadores;
    int totalBolillas = 0;
    int cartonLLeno = 0;
    int bolillas[100];
    int numeros[100] = {0};
    int size = 0;

    if (argc != 2)
    {
        printf("Uso: %s <número de hilos>\n", argv[0]);
        return 1;
    }

    srand(time(NULL));

    cantidad = atoi(argv[1]);
    idHilo = (pthread_t *)malloc(sizeof(pthread_t) * cantidad);
    jugadores = (Jugador *)malloc(sizeof(Jugador) * cantidad);

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_attr_init(&atributos);
    pthread_attr_setdetachstate(&atributos, PTHREAD_CREATE_JOINABLE);

    for (i = 0; i < cantidad; i++)
    {
        jugadores[i].id = i + 1;
        jugadores[i].aciertos = 0;
        jugadores[i].bolillasVerificadas = 0;
        jugadores[i].initCarton = 0;
        jugadores[i].cartonLLeno = &cartonLLeno;
        jugadores[i].totalBolillas = &totalBolillas;
        jugadores[i].bolillas = bolillas;
        if (pthread_create(&idHilo[i], &atributos, funcionThread, &jugadores[i]) != 0)
        {
            perror("No puedo crear thread");
            exit(-1);
        }
    }

    while (cartonLLeno == 0)
    {
        bolillas[totalBolillas] = numeroAleatorioNoRepetitivo(1, 99, numeros, &size);
        totalBolillas++;
        printf("Bolilla %d: %d\n", totalBolillas, bolillas[totalBolillas - 1]);

        /*Notificar a los jugadores que hay una nueva bolilla*/
        pthread_cond_broadcast(&cond);
        usleep(1000 * numeroAleatorio(500, 5000));
    }

    /*Esperar a que todos los hilos terminen*/
    for (i = 0; i < cantidad; i++)
    {
        pthread_join(idHilo[i], NULL);
    }

    /*Mostrar los resultados*/
    if (cartonLLeno != 0)
    {
        printf("El hilo %d ganó el juego\n", cartonLLeno);
        for (i = 0; i < cantidad; i++)
        {
            printf("Hilo %d acerto %d veces\n", jugadores[i].id, jugadores[i].aciertos);
        }
    }

    free(idHilo);
    return 0;
}
