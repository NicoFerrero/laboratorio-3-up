#ifndef _THREAD_H
#define _THREAD_H
#include <pthread.h>

typedef struct
{
    int id;
    int numeroAdivinar;
    int *alguienAdivino;
    int intentos;
} ThreadData;

void *funcionThread(void *parametro);

#endif
