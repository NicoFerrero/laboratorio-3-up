#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <pthread.h>
#include "def.h"
#include "memoria.h"
#include "cola.h"
#include "global.h"
#include "funciones.h"

int main(int argc, char *argv[])
{
    int id_memoria_semaforo;
    int id_cola_mensajes;
    int id_memoria_pateadores;
    int i;
    int patearon = 0;
    int evento = 0;
    int goles = 0;
    mensaje msg;
    semaforo *estadoSemaforo = NULL;
    Pateador *pateadores = NULL;

    estadoSemaforo = (semaforo *)creo_memoria(sizeof(semaforo), &id_memoria_semaforo, CLAVE_BASE);
    pateadores = (Pateador *)creo_memoria(sizeof(Pateador) * CANT_PATEADORES, &id_memoria_pateadores, CLAVE_BASE + 1);

    estadoSemaforo->activoArquero = 1;

    while (estadoSemaforo->activoPateador != 1)
    {
        printf("ESPERANDO QUE INICIE PROCESO PATEADOR\n");
        sleep(1);
    }

    id_cola_mensajes = creoIdColaMensajes(CLAVE_BASE);
    borrarMensajes(id_cola_mensajes); /*Borra todos los mensajes que haya en la cola.*/

    srand(time(NULL));

    while (patearon < 5)
    {
        recibirMensaje(id_cola_mensajes, MSG_ARQUERO, &msg);
        switch (msg.int_evento)
        {
        case EVT_TIRO:
            printf("Recibi un tiro del jugador %s\n", pateadores[msg.int_rte - MSG_PATEADOR].nombre);

            patearon = patearon + 1;
            evento = numeroAleatorio(2, 5);
            if (evento == 4)
            {
                goles = goles + 1;
            }
            enviarMensaje(id_cola_mensajes, msg.int_rte, MSG_ARQUERO, evento, "");
            break;
        default:
            break;
        }
    }

    printf("Recibi %d goles\n", goles);
    for (i = 0; i < CANT_PATEADORES; i++)
    {
        if (pateadores[i].eventoTiro == 4)
        {
            printf("El pateador %s me metio gol\n", pateadores[i].nombre);
        }
    }
    shmdt((char *)estadoSemaforo);
    shmdt((char *)pateadores);
    shmctl(id_memoria_semaforo, IPC_RMID, (struct shmid_ds *)NULL);
    shmctl(id_memoria_pateadores, IPC_RMID, (struct shmid_ds *)NULL);

    return 0;
}
