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
    int id_memoria_semaforo;
    int id_cola_mensajes;
    int numero_pasos;
    int done = 0;
    int i;
    mensaje msg;
    semaforo *estadoSemaforo = NULL;
    Jugador ganadores[3] = {0};
    int ganador = 0;

    estadoSemaforo = (semaforo *)creo_memoria(sizeof(semaforo), &id_memoria_semaforo, CLAVE_BASE);

    estadoSemaforo->activo = 1;

    id_cola_mensajes = creoIdColaMensajes(CLAVE_BASE);

    borrarMensajes(id_cola_mensajes); /*Borra todos los mensajes que haya en la cola.*/

    while (done < 3)
    {
        recibirMensaje(id_cola_mensajes, MSG_PISTA, &msg); /*bloqueate*/

        switch (msg.int_evento)
        {
        case EVT_CORRO:
            numero_pasos = atoi(msg.char_mensaje);

            if (numero_pasos >= 117)
            {
                enviarMensaje(id_cola_mensajes, msg.int_rte, MSG_PISTA, EVT_GANADOR, "");
                ganador = 1;
            }
            else if (numero_pasos < 117 && ganador == 0)
            {
                enviarMensaje(id_cola_mensajes, msg.int_rte, MSG_PISTA, EVT_SEGUIR, "");
            }
            break;
        case EVT_FIN:
            numero_pasos = atoi(msg.char_mensaje);

            if (numero_pasos >= 117)
            {
                ganadores[msg.int_rte - 1].tipo = msg.int_rte;
                ganadores[msg.int_rte - 1].pasos = numero_pasos;
            }
            done = done + 1;
            break;
        default:
            printf("Tablero:%d Evento sin definir\n", msg.int_evento);
            break;
        }
    };
    printf("LOS GANADORES SON\n");
    for (i = 0; i < 3; i++)
    {
        switch (ganadores[i].tipo)
        {
        case CONEJO:
            printf("Jugador: CONEJO con %d pasos\n", ganadores[i].pasos);
            break;
        case GATO:
            printf("Jugador: GATO con %d pasos\n", ganadores[i].pasos);
            break;
        case PERRO:
            printf("Jugador: PERRO con %d pasos\n", ganadores[i].pasos);
            break;
        }
    }

    shmdt((char *)estadoSemaforo);
    shmctl(id_memoria_semaforo, IPC_RMID, (struct shmid_ds *)NULL);
    borrarColaMensajes(id_cola_mensajes);

    return 0;
}