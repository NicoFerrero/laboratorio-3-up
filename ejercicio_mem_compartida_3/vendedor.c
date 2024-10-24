#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include "def.h"
#include "semaforo.h"
#include "memoria.h"
#include "funciones.h"
#include "archivos.h"

volatile sig_atomic_t stop;

void handleSiginit(int sig)
{
    stop = 1;
}

int main(int argc, char *argv[])
{
    int id_memoria;
    int id_memoria_semaforo;
    dato *memoria = NULL;
    semaforo *estadoSemaforo = NULL;
    int idSemaforo, codigo = 0, cantidad = 0, cantidadAux;

    signal(SIGINT, handleSiginit);

    srand(time(NULL));

    idSemaforo = crearSemaforo();
    iniciarSemaforo(idSemaforo, VERDE);

    /*EL sizeof(dato) * CANTIDAD es porque quiero un array de estructura dato*/
    /*Como usamos misma clave y comando para crear clave vamos a obtener el mismo segmento de memoria*/
    memoria = (dato *)creo_memoria(sizeof(dato) * CANTIDAD, &id_memoria, CLAVE_BASE);
    estadoSemaforo = (semaforo *)creo_memoria(sizeof(semaforo), &id_memoria_semaforo, 34);

    while (estadoSemaforo->activo != 1)
    {
        printf("ESPERANDO QUE INICIE PROCESO PEAJE\n");
        sleep(1);
    }

    while (!stop)
    {
        esperarSemaforo(idSemaforo);
        while (codigo < 1 || codigo > 5)
        {
            printf("Ingrese codigo (1 - 5):");
            scanf("%d", &codigo);
        }
        while (cantidad < 1 || cantidad > 5)
        {
            printf("Ingrese una cantidad (1 - 5):");
            scanf("%d", &cantidad);
        }
        cantidadAux = memoria[codigo - 1].cantidad - cantidad;
        if (cantidadAux < 0)
        {
            memoria[codigo - 1].cantidad = 0;
        }
        else
        {
            memoria[codigo - 1].cantidad = memoria[codigo - 1].cantidad - cantidad;
        }
        codigo = 0;
        cantidad = 0;
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
