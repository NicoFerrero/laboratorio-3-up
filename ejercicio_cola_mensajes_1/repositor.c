#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <signal.h>
#include "def.h"
#include "semaforo.h"
#include "memoria.h"
#include "funciones.h"
#include "cola.h"

volatile sig_atomic_t stop;

void handleSiginit(int sig)
{
    stop = 1;
}

void procesar_evento(int id_cola_mensajes, mensaje msg)
{

    printf("Destino   %d\n", (int)msg.long_dest);
    printf("Remitente %d\n", msg.int_rte);
    printf("Evento    %d\n", msg.int_evento);
    printf("Mensaje   %s\n", msg.char_mensaje);
    switch (msg.int_evento)
    {
    case EVT_RESPUESTA_STOCK:
        printf("Rta stock\n");
        printf("STOCK %d\n", atoi(msg.char_mensaje));
        break;

    default:
        printf("\nEvento sin definir\n");
        break;
    }
    printf("------------------------------\n");
}

int main(int argc, char *argv[])
{
    int id_cola_mensajes;
    mensaje msg;

    id_cola_mensajes = creoIdColaMensajes(CLAVE_BASE);
    borrarMensajes(id_cola_mensajes);

    while (!stop)
    {
        enviarMensaje(id_cola_mensajes, MSG_SUPERMERCADO, MSG_REPOSITOR, EVT_SUMA_STOCK, "SUMA UNO");
        enviarMensaje(id_cola_mensajes, MSG_SUPERMERCADO, MSG_REPOSITOR, EVT_CONSULTA_STOCK, "DECIME EL STOCK POR FAVOR");
        recibirMensaje(id_cola_mensajes, MSG_REPOSITOR, &msg);
        procesar_evento(id_cola_mensajes, msg);
        usleep(2000 * 1000);
    };

    borrarColaMensajes(id_cola_mensajes);

    return 0;
}
