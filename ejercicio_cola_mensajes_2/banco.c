#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include "def.h"
#include "semaforo.h"
#include "memoria.h"
#include "funciones.h"
#include "cola.h"

cliente clientes[100];

void cargarClientes()
{
    int i;
    for (i = 0; i < 100; i++)
    {
        clientes[i].id = i + 1;
        clientes[i].saldo = numeroAleatorio(1000, 10000);
    }
}

int buscarCliente(int id)
{
    int i;
    for (i = 0; i < 100; i++)
    {
        if (clientes[i].id == id)
        {
            return i;
        }
    }
    return -1;
}

void procesar_evento(int id_cola_mensajes, mensaje msg)
{
    char cadena[100];
    int saldo = 0;
    int importe = 0;
    int id = 0;
    char *token;
    int count = 0;
    int index = 0;
    /*printf("Destino   %d\n", (int)msg.long_dest);
    printf("Remitente %d\n", msg.int_rte);
    printf("Evento    %d\n", msg.int_evento);
    printf("Mensaje   %s\n", msg.char_mensaje);*/
    switch (msg.int_evento)
    {
    case EVT_CONSULTA_SALDO:
        printf("Consulta Saldo\n");
        id = atoi(msg.char_mensaje);
        index = buscarCliente(id);
        if (index < 0)
        {
            sprintf(cadena, "%d|%s", id, "Cliente no encontrado");
            enviarMensaje(id_cola_mensajes, msg.int_rte, MSG_BANCO, EVT_RTA_SALDO_NOK, cadena);
        }
        else
        {
            saldo = clientes[index].saldo;
            sprintf(cadena, "%d|%d", id, saldo);
            enviarMensaje(id_cola_mensajes, msg.int_rte, MSG_BANCO, EVT_RTA_SALDO, cadena);
        }
        break;
    case EVT_DEPOSITO:
        printf("Deposito\n");
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
        index = buscarCliente(id);
        if (index < 0)
        {
            sprintf(cadena, "%d|%s", id, "Cliente no encontrado");
            enviarMensaje(id_cola_mensajes, msg.int_rte, MSG_BANCO, EVT_RTA_DEPOSITO_NOK, cadena);
        }
        else
        {
            clientes[index].saldo = clientes[index].saldo + importe;
            sprintf(cadena, "%d|%d", id, clientes[index].saldo);
            enviarMensaje(id_cola_mensajes, msg.int_rte, MSG_BANCO, EVT_RTA_DEPOSITO_OK, cadena);
        }
        break;
    case EVT_EXTRACCION:
        printf("Extraccion\n");
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
        index = buscarCliente(id);
        if (index < 0)
        {
            sprintf(cadena, "%d|%s", id, "Cliente no encontrado");
            enviarMensaje(id_cola_mensajes, msg.int_rte, MSG_BANCO, EVT_RTA_EXTRACCION_NOK, cadena);
        }
        else
        {
            if (clientes[index].saldo - importe >= 0)
            {
                clientes[index].saldo = clientes[index].saldo - importe;
                sprintf(cadena, "%d|%d", id, clientes[index].saldo);
                enviarMensaje(id_cola_mensajes, msg.int_rte, MSG_BANCO, EVT_RTA_EXTRACCION_OK, cadena);
            }
            else
            {
                sprintf(cadena, "%d|%s", id, "No puede retirar mas de lo que tieen en su cuenta");
                enviarMensaje(id_cola_mensajes, msg.int_rte, MSG_BANCO, EVT_RTA_EXTRACCION_NOK, cadena);
            }
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

    id_cola_mensajes = creoIdColaMensajes(CLAVE_BASE);
    borrarMensajes(id_cola_mensajes);
    cargarClientes();

    while (msg.int_evento != 0)
    {
        recibirMensaje(id_cola_mensajes, MSG_BANCO, &msg);
        procesar_evento(id_cola_mensajes, msg);
    };

    borrarColaMensajes(id_cola_mensajes);

    return 0;
}
