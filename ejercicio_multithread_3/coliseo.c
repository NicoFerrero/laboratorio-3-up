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
    int id_memoria_luchadores;
    int id_cola_mensajes;
    int ataques = 0;
    int turno = 0;
    int ataqueRecibido = 0;
    int ganaronGladiadores = 0;
    int ganaronLuchadoresGladiadores = 0;
    int i;
    mensaje msg;
    semaforo *estadoSemaforo = NULL;
    Luchador *luchadores = NULL;

    estadoSemaforo = (semaforo *)creo_memoria(sizeof(semaforo), &id_memoria_semaforo, CLAVE_BASE);
    luchadores = (Luchador *)creo_memoria(sizeof(Luchador) * CANT_LUCHADORES, &id_memoria_luchadores, CLAVE_BASE + 1);

    estadoSemaforo->activo = 1;

    id_cola_mensajes = creoIdColaMensajes(CLAVE_BASE);

    srand(time(NULL));

    borrarMensajes(id_cola_mensajes); /*Borra todos los mensajes que haya en la cola.*/

    while (ganaronGladiadores == ganaronLuchadoresGladiadores)
    {
        if (ataques == 8 || ataques == 0)
        {
            usleep(1000 * 200);
            if (ataques == 8)
            {
                printf("***********FIN TURNO %d***********\n", turno);
            }
            ataques = 0;
            turno = turno + 1;
            printf("***********TURNO %d***********\n", turno);
            for (i = 0; i < CANT_LUCHADORES; i++)
            {
                enviarMensaje(id_cola_mensajes, MSG_LUCHADOR + i, MSG_COLISEO, EVT_TURNO, "");
            }
        }
        recibirMensaje(id_cola_mensajes, MSG_COLISEO, &msg);
        switch (msg.int_evento)
        {
        case EVT_ATAQUE:
            ataques = ataques + 1;
            ataqueRecibido = atoi(msg.char_mensaje);
            if (MSG_LUCHADOR - msg.int_rte >= 0 && MSG_LUCHADOR - msg.int_rte < 3)
            {
                if (MSG_LUCHADOR - msg.int_rte == 0)
                {
                    printf("%s va a atacar con %d de danio\n", luchadores[MSG_LUCHADOR - msg.int_rte].nombre, ataqueRecibido);
                }
                else
                {
                    printf("El %s va a atacar con %d de danio\n", luchadores[MSG_LUCHADOR - msg.int_rte].nombre, ataqueRecibido);
                }
                for (i = 3; i < CANT_LUCHADORES; i++)
                {
                    luchadores[i].vida = luchadores[i].vida - ataqueRecibido;
                    printf("El Luchador gladiador %d recibio %d puntos de danio. Le quedan %d puntos de vida\n", i - 2, ataqueRecibido, luchadores[i].vida);
                    if (luchadores[i].vida <= 0)
                    {
                        enviarMensaje(id_cola_mensajes, MSG_LUCHADOR + i, MSG_COLISEO, EVT_MUERTO, "");
                        ganaronGladiadores = 1;
                    }
                }
            }
            else
            {
                printf("%s va a atacar con %d de danio\n", luchadores[MSG_LUCHADOR + msg.int_rte].nombre, ataqueRecibido);
                for (i = 0; i < CANT_LUCHADORES - 5; i++)
                {
                    luchadores[i].vida = luchadores[i].vida - ataqueRecibido;
                    if (i == 0)
                    {
                        printf("Maximo recibio %d puntos de danio. Le quedan %d puntos de vida\n", ataqueRecibido, luchadores[i].vida);
                    }
                    else
                    {
                        printf("El gladiador %d recibio %d puntos de danio. Le quedan %d puntos de vida\n", i, ataqueRecibido, luchadores[i].vida);
                    }
                    if (luchadores[i].vida <= 0)
                    {
                        enviarMensaje(id_cola_mensajes, MSG_LUCHADOR + i, MSG_COLISEO, EVT_MUERTO, "");
                        ganaronLuchadoresGladiadores = 1;
                    }
                }
            }
            break;
        default:
            break;
        }
    }

    printf("***********FIN TURNO %d***********\n", turno);
    if (ganaronGladiadores)
    {
        printf("Ganaron los gladiadores. El emperador va a decidir si viven o no\n");
        if (numeroAleatorio(0, 1) == 0)
        {
            printf("El emperador decidio que van a morir\n");
            for (i = 0; i < CANT_LUCHADORES - 5; i++)
            {
                enviarMensaje(id_cola_mensajes, MSG_LUCHADOR + i, MSG_COLISEO, EVT_MUERTO, "");
            }
        }
        else
        {
            printf("El emperador decidio que van a vivir\n");
            for (i = 0; i < CANT_LUCHADORES - 5; i++)
            {
                enviarMensaje(id_cola_mensajes, MSG_LUCHADOR + i, MSG_COLISEO, EVT_GANADOR, "");
            }
        }
    }
    else
    {
        for (i = 3; i < CANT_LUCHADORES; i++)
        {
            enviarMensaje(id_cola_mensajes, MSG_LUCHADOR + i, MSG_COLISEO, EVT_GANADOR, "");
        }
        printf("Ganaron los luchadores gladiadores\n");
    }

    shmdt((char *)estadoSemaforo);
    shmdt((char *)luchadores);
    shmctl(id_memoria_semaforo, IPC_RMID, (struct shmid_ds *)NULL);
    shmctl(id_memoria_luchadores, IPC_RMID, (struct shmid_ds *)NULL);

    return 0;
}
