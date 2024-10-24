#ifndef _THREAD_H
#define _THREAD_H
#include <pthread.h>

typedef struct
{
    int id;
    int carton[5];
    int aciertos;
    int bolillasVerificadas;
    int initCarton;
    int *cartonLLeno;
    int *bolillas;
    int *totalBolillas;
} Jugador;

void *funcionThread(void *parametro);

#endif
