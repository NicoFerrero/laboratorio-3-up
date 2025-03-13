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
    int id_memoria_corredores;
    int id_cola_mensajes;
    int i;
    int done = 0;
    mensaje msg;
    semaforo *estadoSemaforo = NULL;
    Corredor *corredores = NULL;
    int distanciaCorredores[CANT_CORREDORES];
    int checkBoxes[CANT_CORREDORES];
    int distancia = 0;
    int participantesEnPista = 10;

    for (i = 0; i < CANT_CORREDORES; i++)
    {
        distanciaCorredores[i] = 0;
        checkBoxes[i] = 0;
    }

    estadoSemaforo = (semaforo *)creo_memoria(sizeof(semaforo), &id_memoria_semaforo, CLAVE_BASE);
    corredores = (Corredor *)creo_memoria(sizeof(Corredor) * CANT_CORREDORES, &id_memoria_corredores, CLAVE_BASE + 1);

    estadoSemaforo->activoCarrera = 1;

    while (estadoSemaforo->activoCorredor != 1)
    {
        printf("ESPERANDO QUE INICIE PROCESO CORREDOR\n");
        sleep(1);
    }

    id_cola_mensajes = creoIdColaMensajes(CLAVE_BASE);

    srand(time(NULL));

    borrarMensajes(id_cola_mensajes); /*Borra todos los mensajes que haya en la cola.*/

    for (i = 0; i < CANT_CORREDORES; i++)
    {
        enviarMensaje(id_cola_mensajes, MSG_CORREDOR + i, MSG_CARRERA, EVT_CORRER, "");
    }

    while (done == 0 && participantesEnPista > 0)
    {
        recibirMensaje(id_cola_mensajes, MSG_CARRERA, &msg);
        switch (msg.int_evento)
        {
        case EVT_AVANZAR:
            distancia = atoi(msg.char_mensaje);
            if (distancia == 0)
            {
                printf("El equipo %s entro a boxes\n", corredores[msg.int_rte - MSG_CORREDOR].nombre);
                enviarMensaje(id_cola_mensajes, msg.int_rte, MSG_CARRERA, EVT_CORRER, "");
                break;
            }
            distanciaCorredores[msg.int_rte - MSG_CORREDOR] = distanciaCorredores[msg.int_rte - MSG_CORREDOR] + distancia;
            printf("Soy el equipo %s y avance %d metros. En total llevo %d metros\n", corredores[msg.int_rte - MSG_CORREDOR].nombre, distancia, distanciaCorredores[msg.int_rte - MSG_CORREDOR]);
            if (distanciaCorredores[msg.int_rte - MSG_CORREDOR] >= 295 && estadoSemaforo->ganador == 0)
            {
                enviarMensaje(id_cola_mensajes, msg.int_rte, MSG_CARRERA, EVT_GANADOR, "");
                estadoSemaforo->ganador = 1;
                printf("El equipo %s gano la carrera\n", corredores[msg.int_rte - MSG_CORREDOR].nombre);
                for (i = 0; i < CANT_CORREDORES; i++)
                {
                    if (MSG_CORREDOR + i != msg.int_rte && corredores[msg.int_rte - MSG_CORREDOR].termine == 0)
                    {
                        enviarMensaje(id_cola_mensajes, MSG_CORREDOR + i, MSG_CARRERA, EVT_PERDEDOR, "");
                    }
                }
                participantesEnPista = 0;
                done = 1;
                break;
            }
            /* Ver si sale de pista y pierde la carrera o solo choco*/
            if (numeroAleatorio(1, 100) == 2)
            {
                printf("El equipo %s salio de pista. Queda fuera de la carrera\n", corredores[msg.int_rte - MSG_CORREDOR].nombre);
                enviarMensaje(id_cola_mensajes, msg.int_rte, MSG_CARRERA, EVT_SALIO_PISTA, "");
                participantesEnPista = participantesEnPista - 1;
            }
            else
            {

                if ((distanciaCorredores[msg.int_rte - MSG_CORREDOR] >= 50 && checkBoxes[msg.int_rte - MSG_CORREDOR] < 1) ||
                    (distanciaCorredores[msg.int_rte - MSG_CORREDOR] >= 100 && checkBoxes[msg.int_rte - MSG_CORREDOR] < 2) ||
                    (distanciaCorredores[msg.int_rte - MSG_CORREDOR] >= 150 && checkBoxes[msg.int_rte - MSG_CORREDOR] < 3) ||
                    (distanciaCorredores[msg.int_rte - MSG_CORREDOR] >= 200 && checkBoxes[msg.int_rte - MSG_CORREDOR] < 4) ||
                    (distanciaCorredores[msg.int_rte - MSG_CORREDOR] >= 250 && checkBoxes[msg.int_rte - MSG_CORREDOR] < 5))
                {
                    checkBoxes[msg.int_rte - MSG_CORREDOR] = checkBoxes[msg.int_rte - MSG_CORREDOR] + 1;
                    if (numeroAleatorio(1, 3) == 1)
                    {
                        printf("El equipo %s entro a boxes. El proximo avance sera de 0 metros\n", corredores[msg.int_rte - MSG_CORREDOR].nombre);
                        enviarMensaje(id_cola_mensajes, msg.int_rte, MSG_CARRERA, EVT_BOXES, "");
                    }
                    else
                    {
                        printf("El equipo %s no entro a boxes y sigue corriendo\n", corredores[msg.int_rte - MSG_CORREDOR].nombre);
                        enviarMensaje(id_cola_mensajes, msg.int_rte, MSG_CARRERA, EVT_CORRER, "");
                    }
                }
                else
                {
                    enviarMensaje(id_cola_mensajes, msg.int_rte, MSG_CARRERA, EVT_CHOCO, "");
                    printf("El equipo %s choco pero sigue en carrera\n", corredores[msg.int_rte - MSG_CORREDOR].nombre);
                }
            }
            break;
        default:
            break;
        }
    }

    printf("TABLA DE GANADORES\nESCUDERIA\tMETROS\n");
    for (i = 0; i < CANT_CORREDORES; i++)
    {
        printf("%s\t\t%d\n", corredores[i].nombre, distanciaCorredores[i]);
    }

    shmdt((char *)estadoSemaforo);
    shmdt((char *)corredores);
    shmctl(id_memoria_semaforo, IPC_RMID, (struct shmid_ds *)NULL);
    shmctl(id_memoria_corredores, IPC_RMID, (struct shmid_ds *)NULL);

    return 0;
}
