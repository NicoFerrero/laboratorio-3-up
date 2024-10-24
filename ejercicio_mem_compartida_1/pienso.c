#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include "def.h"
#include "semaforo.h"
#include "memoria.h"
#include "funciones.h"

int main(int argc, char *argv[])
{
    int id_memoria;
    int id_memoria_semaforo;
    dato *memoria = NULL;
    semaforo *estadoSemaforo = NULL;
    int piensoUnNumero = 80;
    int idSemaforo;
    int adivino = 0;

    srand(time(NULL));

    idSemaforo = crearSemaforo();
    iniciarSemaforo(idSemaforo, VERDE);

    /*EL sizeof(dato) * CANTIDAD es porque quiero un array de estructura dato*/
    /*Como usamos misma clave y comando para crear clave vamos a obtener el mismo segmento de memoria*/
    memoria = (dato *)creo_memoria(sizeof(dato), &id_memoria, CLAVE_BASE);
    estadoSemaforo = (semaforo *)creo_memoria(sizeof(semaforo), &id_memoria_semaforo, 34);

    estadoSemaforo->activo = 1;
    memoria->estadoAcierto = 0;
    memoria->numeroPensado = 0;

    while (adivino != 1)
    {
        esperarSemaforo(idSemaforo);
        if (memoria->numeroPensado != 0 && memoria->estadoAcierto == 0)
        {
            if (memoria->numeroPensado == piensoUnNumero)
            {
                memoria->estadoAcierto = 1;
                adivino = 1;
                printf("NUMERO PENSADO: %d\n", piensoUnNumero);
                printf("JUGADOR: %s GANADOR\n", memoria->nombreJugador);
            }
            else
            {
                printf("EL NUMERO %d NO ES EL QUE ESTOY PENSANDO\n", memoria->numeroPensado);
                memoria->numeroPensado = 0;
                memoria->estadoAcierto = 0;
            }
        }
        levantarSemaforo(idSemaforo);
        usleep(1000 * 500);
    }
    /*DEBEMOS ASEGURARNOS QUE ESTO EN ALGUN MOMENTO SE VA A EJECUTAR, CON EL WHILE 1 NUCNA LO HARA*/
    shmdt((char *)memoria);
    shmctl(id_memoria, IPC_RMID, (struct shmid_ds *)NULL);
    shmdt((char *)estadoSemaforo);
    shmctl(id_memoria_semaforo, IPC_RMID, (struct shmid_ds *)NULL);
    return 0;
}
