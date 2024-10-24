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
#include "cola.h"

void procesar_evento(int id_cola_mensajes, mensaje msg)
{
    char *token;
    int id = 0;
    int saldo = 0;
    int importe = 0;
    char error[100];
    int count = 0;
    /*printf("Destino   %d\n", (int)msg.long_dest);
    printf("Remitente %d\n", msg.int_rte);
    printf("Evento    %d\n", msg.int_evento);
    printf("Mensaje   %s\n", msg.char_mensaje);*/
    switch (msg.int_evento)
    {
    case EVT_RTA_SALDO:
        printf("Rta saldo\n");
        count = 0;
        token = NULL;
        token = strtok(msg.char_mensaje, "|");
        while (token != NULL)
        {
            switch (count)
            {
            case 0:
                id = atoi(token);
                printf("ID:%d", id);
                break;
            case 1:
                saldo = atoi(token);
                printf(" Saldo:%d\n", saldo);
                break;
            }
            token = strtok(NULL, "|");
            count++;
        }
        break;
    case EVT_RTA_SALDO_NOK:
        printf("Rta saldo no ok\n");
        count = 0;
        token = NULL;
        token = strtok(msg.char_mensaje, "|");
        while (token != NULL)
        {
            switch (count)
            {
            case 0:
                id = atoi(token);
                printf("ID:%d", id);
                break;
            case 1:
                memset(error, 0x00, sizeof(error));
                strcpy(error, token);
                printf(" Error:%s\n", error);
                break;
            }
            token = strtok(NULL, "|");
            count++;
        }
        break;
    case EVT_RTA_DEPOSITO_OK:
        printf("Rta deposito\n");
        count = 0;
        token = NULL;
        token = strtok(msg.char_mensaje, "|");
        while (token != NULL)
        {
            switch (count)
            {
            case 0:
                id = atoi(token);
                printf("ID:%d", id);
                break;
            case 1:
                importe = atoi(token);
                printf(" Importe:%d\n", importe);
                break;
            }
            token = strtok(NULL, "|");
            count++;
        }
        break;
    case EVT_RTA_DEPOSITO_NOK:
        printf("Rta deposito no ok\n");
        count = 0;
        token = NULL;
        token = strtok(msg.char_mensaje, "|");
        while (token != NULL)
        {
            switch (count)
            {
            case 0:
                id = atoi(token);
                printf("ID:%d", id);
                break;
            case 1:
                memset(error, 0x00, sizeof(error));
                strcpy(error, token);
                printf(" Error:%s\n", error);
                break;
            }
            token = strtok(NULL, "|");
            count++;
        }
        break;
    case EVT_RTA_EXTRACCION_OK:
        printf("Rta extraccion\n");
        count = 0;
        token = NULL;
        token = strtok(msg.char_mensaje, "|");
        while (token != NULL)
        {
            switch (count)
            {
            case 0:
                id = atoi(token);
                printf("ID:%d", id);
                break;
            case 1:
                importe = atoi(token);
                printf(" Importe:%d\n", importe);
                break;
            }
            token = strtok(NULL, "|");
            count++;
        }
        break;
    case EVT_RTA_EXTRACCION_NOK:
        printf("Rta extraccion no ok\n");
        count = 0;
        token = NULL;
        token = strtok(msg.char_mensaje, "|");
        while (token != NULL)
        {
            switch (count)
            {
            case 0:
                id = atoi(token);
                printf("ID:%d", id);
                break;
            case 1:
                memset(error, 0x00, sizeof(error));
                strcpy(error, token);
                printf(" Error:%s\n", error);
                break;
            }
            token = strtok(NULL, "|");
            count++;
        }
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
    int opcion = -1;
    char mensaje[100];
    int id = 0;
    int importe = 0;

    id_cola_mensajes = creoIdColaMensajes(CLAVE_BASE);

    while (opcion != 0)
    {
        printf("****MENU*****\n");
        printf("0- Salir\n");
        printf("1- Consultar saldo\n");
        printf("2- Hacer deposito\n");
        printf("3- Hacer extraccion\n");
        scanf("%d", &opcion);
        switch (opcion)
        {
        case 0:
            enviarMensaje(id_cola_mensajes, MSG_BANCO, MSG_CAJERO, EVT_NINGUNO, "");
            break;
        case 1:
            printf("Ingrese su id: ");
            scanf("%d", &id);
            memset(mensaje, 0x00, sizeof(mensaje));
            sprintf(mensaje, "%d", id);
            enviarMensaje(id_cola_mensajes, MSG_BANCO, MSG_CAJERO, EVT_CONSULTA_SALDO, mensaje);
            break;
        case 2:
            printf("Ingrese su id: ");
            scanf("%d", &id);
            printf("Ingrese el importe a depositar: ");
            scanf("%d", &importe);
            memset(mensaje, 0x00, sizeof(mensaje));
            sprintf(mensaje, "%d|%d", id, importe);
            enviarMensaje(id_cola_mensajes, MSG_BANCO, MSG_CAJERO, EVT_DEPOSITO, mensaje);
            break;
        case 3:
            printf("Ingrese su id: ");
            scanf("%d", &id);
            printf("Ingrese el importe a retirar: ");
            scanf("%d", &importe);
            memset(mensaje, 0x00, sizeof(mensaje));
            sprintf(mensaje, "%d|%d", id, importe);
            enviarMensaje(id_cola_mensajes, MSG_BANCO, MSG_CAJERO, EVT_EXTRACCION, mensaje);
            break;

        default:
            break;
        }
        if (opcion != 0)
        {
            recibirMensaje(id_cola_mensajes, MSG_CAJERO, &msg);
            procesar_evento(id_cola_mensajes, msg);
            usleep(2000 * 1000);
        }
    };

    borrarColaMensajes(id_cola_mensajes);

    return 0;
}
