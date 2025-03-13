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
    int tiro;
    int done = 0;
    int i;
    char mensajeBolera[LARGO_CADENA];
    mensaje msg;
    semaforo *estadoSemaforo = NULL;
    int lineaBolos[CANT_JUGADORES];
    int turnosJugadores[CANT_JUGADORES];
    int puntosJugadores[CANT_JUGADORES];

    srand(time(NULL));

    estadoSemaforo = (semaforo *)creo_memoria(sizeof(semaforo), &id_memoria_semaforo, CLAVE_BASE);

    estadoSemaforo->activo = 1;

    id_cola_mensajes = creoIdColaMensajes(CLAVE_BASE);

    borrarMensajes(id_cola_mensajes); /*Borra todos los mensajes que haya en la cola.*/

    for (i = 0; i < CANT_JUGADORES; i++)
    {
        lineaBolos[i] = 10;
        turnosJugadores[i] = 0;
        puntosJugadores[i] = 0;
        printf("Enviando evento de inicio a jugador %d\n", MSG_JUGADOR + i - 1);
        enviarMensaje(id_cola_mensajes, MSG_JUGADOR + i, MSG_BOLERA, EVT_INICIO, "");
    }
    while (done < MSG_JUGADOR)
    {
        recibirMensaje(id_cola_mensajes, MSG_BOLERA, &msg);
        switch (msg.int_evento)
        {
        case EVT_TIRO:
            turnosJugadores[msg.int_rte - MSG_JUGADOR] = turnosJugadores[msg.int_rte - MSG_JUGADOR] + 1;
            tiro = numeroAleatorio(1, lineaBolos[msg.int_rte - MSG_JUGADOR]);
            if (lineaBolos[msg.int_rte - MSG_JUGADOR] == 10 && tiro == 10)
            {
                printf("El jugador %d hizo una CHUZA\n", msg.int_rte - 1);
                puntosJugadores[msg.int_rte - MSG_JUGADOR] = 20;
            }
            else
            {
                printf("El jugador %d tiro %d bolos\n", msg.int_rte - 1, tiro);
                puntosJugadores[msg.int_rte - MSG_JUGADOR] = puntosJugadores[msg.int_rte - MSG_JUGADOR] + tiro * 1;
            }

            lineaBolos[msg.int_rte - MSG_JUGADOR] = lineaBolos[msg.int_rte - MSG_JUGADOR] - tiro;
            if (lineaBolos[msg.int_rte - MSG_JUGADOR] > 0 && turnosJugadores[msg.int_rte - MSG_JUGADOR] < 3)
            {
                printf("Al jugador %d le quedan %d bolos por tirar\n", msg.int_rte - 1, lineaBolos[msg.int_rte - MSG_JUGADOR]);
                sprintf(mensajeBolera, "%d", tiro);
                enviarMensaje(id_cola_mensajes, msg.int_rte, MSG_BOLERA, EVT_RESULTADO, mensajeBolera);
            }
            else
            {
                printf("El jugador %d termino la partida con %d bolos por tirar\n", msg.int_rte - 1, lineaBolos[msg.int_rte - MSG_JUGADOR]);
                sprintf(mensajeBolera, "%d", puntosJugadores[msg.int_rte - MSG_JUGADOR]);
                enviarMensaje(id_cola_mensajes, msg.int_rte, MSG_BOLERA, EVT_FIN, mensajeBolera);
                done = done + 1;
            }

            break;
        default:
            break;
        }
    }

    printf("Tabla de posiciones\nJUGADOR\tPUNTOS\tTURNOS\n");

    for (i = 0; i < CANT_JUGADORES; i++)
    {
        printf("%d\t%d\t%d\n", i + 1, puntosJugadores[i], turnosJugadores[i]);
    }

    shmdt((char *)estadoSemaforo);
    shmctl(id_memoria_semaforo, IPC_RMID, (struct shmid_ds *)NULL);

    return 0;
}
