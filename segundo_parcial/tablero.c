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
    int done = 0;
    int i;
    int humo = 0;
    int avance;
    int avanzarHumo = 0;
    int avanceMosquitos[CANT_JUGADORES];
    mensaje msg;
    semaforo *estadoSemaforo = NULL;

    estadoSemaforo = (semaforo *)creo_memoria(sizeof(semaforo), &id_memoria_semaforo, CLAVE_BASE);

    estadoSemaforo->activoEspiral = 1;

    while (estadoSemaforo->activoEspiral != 1 || estadoSemaforo->activoJugadores != 1)
    {
        printf("ESPIRAL: ESPERANDO QUE INICIEN AMBOS PROCESOS\n");
        sleep(1);
    }

    srand(time(NULL));

    id_cola_mensajes = creoIdColaMensajes(CLAVE_BASE);

    borrarMensajes(id_cola_mensajes); /*Borra todos los mensajes que haya en la cola.*/

    humo = humo + numeroAleatorio(1, 4);
    printf("El humo avanzo %d metros\n", humo);

    for (i = 0; i < CANT_JUGADORES; i++)
    {
        enviarMensaje(id_cola_mensajes, MSG_JUGADOR + i, MSG_HUMO, EVT_INI_MOSQUITO, "");
    }

    while (done < CANT_JUGADORES)
    {
        recibirMensaje(id_cola_mensajes, MSG_HUMO, &msg); /*bloqueate*/
        switch (msg.int_evento)
        {
        case EVT_AVZ_MOSQUITO:
            avance = atoi(msg.char_mensaje);
            usleep(1000 * 300);
            if (humo >= avance && avance < LARGO_ESPIRAL)
            {
                enviarMensaje(id_cola_mensajes, msg.int_rte, MSG_HUMO, EVT_MOSQUITO_PERD, "");
                avanceMosquitos[msg.int_rte - 2] = avance;
                done = done + 1;
            }
            else if (humo < avance && avance < LARGO_ESPIRAL)
            {
                avanzarHumo = avanzarHumo + 1;
                if (avanzarHumo >= 2 - done)
                {
                    avanzarHumo = 0;
                    humo = humo + numeroAleatorio(1, 4);
                    printf("El humo avanzo %d metros\n", humo);
                }
                enviarMensaje(id_cola_mensajes, msg.int_rte, MSG_HUMO, EVT_SEGUIR, "");
            }
            else if (humo < avance && avance >= LARGO_ESPIRAL)
            {
                enviarMensaje(id_cola_mensajes, msg.int_rte, MSG_HUMO, EVT_MOSQUITO_GAN, "");
                avanceMosquitos[msg.int_rte - 2] = avance;
                done = done + 1;
            }
            break;
        default:
            printf("Tablero:%d Evento sin definir\n", msg.int_evento);
            break;
        }
    };
    printf("LOS RESULTADOS SON\n");
    for (i = 0; i < CANT_JUGADORES; i++)
    {
        if (avanceMosquitos[MSG_JUGADOR + i - 2] > humo)
        {

            printf("El mosquito %d avanzo %d metros y el humo %d metros (GANADOR)\n", MSG_JUGADOR + i - 1, avanceMosquitos[MSG_JUGADOR + i - 2], humo);
        }
        else
        {
            printf("El mosquito %d avanzo %d metros y el humo %d metros (PERDEDOR)\n", MSG_JUGADOR + i - 1, avanceMosquitos[MSG_JUGADOR + i - 2], humo);
        }
    }

    shmdt((char *)estadoSemaforo);
    shmctl(id_memoria_semaforo, IPC_RMID, (struct shmid_ds *)NULL);

    return 0;
}
