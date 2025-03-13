#include <stdio.h>
#include <stdlib.h>
#include "sys/ipc.h"
#include "sys/sem.h"
#include "semaforo.h"
#include "clave.h"
#include "def.h"

int crearSemaforo()
{
    key_t clave = crearClave(CLAVE_BASE);
    /*  Parametros de semget:
        1- Clave que debe ser igual si quiero referirme al mismo semaforo
        2- Cantidad de semaforos a crear
        3- Permisos en octal y accion de crear
    */
    int idSemaforo = semget(clave, 1, 0600 | IPC_CREAT);
    if (idSemaforo == -1)
    {
        printf("ERROR: No se pudo crear el semaforo\n");
        exit(0);
    }
    return idSemaforo;
}

void iniciarSemaforo(int idSemaforo, int valorInicial)
{
    semctl(idSemaforo, 0, SETVAL, valorInicial);
}

void esperarSemaforo(int idSemaforo)
{
    struct sembuf operacion;
    printf("ESPERAR SEMAFORO\n");
    operacion.sem_num = 0;
    /*Decremento el semaforo en 1*/
    operacion.sem_op = -1;
    operacion.sem_flg = 0;
    semop(idSemaforo, &operacion, 1);
}

void levantarSemaforo(int idSemaforo)
{
    struct sembuf operacion;
    printf("LEVANTAR SEMAFORO\n");
    operacion.sem_num = 0;
    /*Incremento el semaforo en 1*/
    operacion.sem_op = 1;
    operacion.sem_flg = 0;
    semop(idSemaforo, &operacion, 1);
}
