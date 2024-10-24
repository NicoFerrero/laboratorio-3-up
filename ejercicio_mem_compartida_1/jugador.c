#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/shm.h>
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
    int piensoUnNumero = 0, intentos = 0, size = 0;
    int numerosPensados[100] = {0};
    int idSemaforo;
    int adivino = 0;

    srand(time(NULL));

    idSemaforo = crearSemaforo();

    /*EL sizeof(dato) * CANTIDAD es porque quiero un array de estructura dato*/
    /*Como usamos misma clave y comando para crear clave vamos a obtener el mismo segmento de memoria*/
    memoria = (dato *)creo_memoria(sizeof(dato), &id_memoria, CLAVE_BASE);
    estadoSemaforo = (semaforo *)creo_memoria(sizeof(semaforo), &id_memoria_semaforo, 34);

    while (estadoSemaforo->activo != 1)
    {
        printf("ESPERANDO QUE INICIE PROCESO PIENSO\n");
        sleep(1);
    }

    printf("Ingrese su nombre: ");
    memset(memoria->nombreJugador, 0x00, sizeof(memoria->nombreJugador));
    scanf(" %[^\n]", memoria->nombreJugador);

    while (adivino != 1)
    {
        esperarSemaforo(idSemaforo);
        if (memoria->numeroPensado == 0 && memoria->estadoAcierto == 0)
        {
            piensoUnNumero = numeroAleatorioNoRepetitivo(1, 99, numerosPensados, &size);
            printf("NUMERO PENSADO: %d\n", piensoUnNumero);
            memoria->numeroPensado = piensoUnNumero;
            intentos = intentos + 1;
        }
        else if (memoria->numeroPensado != 0 && memoria->estadoAcierto == 1)
        {
            printf("LA CANTIDAD DE INTENTOS FUE: %d\n", intentos);
            adivino = 1;
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
