#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "def.h"
#include "archivos.h"
#include "semaforo.h"

int main(int argc, char *argv[])
{
    FILE *archivo;
    char *lote;
    int idSemaforo;
    char pedido;
    int total;
    int cantidad;

    if (argc != 2)
    {
        printf("No ingreso la cantidad de parametros requerida. Debe pasar la cantidad de lotes\n");
    }
    pedido = argv[1][0];
    if (pedido != 'a' && pedido != 'b' && pedido != 'c')
    {
        printf("La cocina de este restaurante solo puede funcionar para los menues a, b o c\n");
        return 0;
    }

    switch (pedido)
    {
    case 'a':
        lote = LOTE_A;
        break;
    case 'b':
        lote = LOTE_B;
        break;
    case 'c':
        lote = LOTE_C;
        break;
    default:
        break;
    }
    idSemaforo = crearSemaforo();
    iniciarSemaforo(idSemaforo, VERDE);
    esperarSemaforo(idSemaforo);
    archivo = abrirArchivo(lote, "r");
    while (archivo == NULL)
    {
        archivo = abrirArchivo(lote, "w");
    }
    cerrarArchivo(archivo);
    levantarSemaforo(idSemaforo);

    while (1)
    {
        esperarSemaforo(idSemaforo);
        archivo = abrirArchivo(lote, "r");
        if (archivo != NULL)
        {
            Pedido pedido;
            cantidad = 0;
            total = 0;
            system("clear");
            printf("MENU\tPOSTRE\tPRECIO TOTAL\n");
            while (leerArchivoCompleto(archivo, "%c\t%01d\t%04d\n", &pedido.tipoPedido, &pedido.tienePostre, &pedido.precio))
            {
                printf("%c\t%01d\t%04d\n", pedido.tipoPedido, pedido.tienePostre, pedido.precio);
                cantidad = cantidad + 1;
                total = total + pedido.precio;
            }
            printf("PEDIDOS TOTALES: %d\nTOTAL PEDIDOS: %d\n", cantidad, total);
            cerrarArchivo(archivo);
            levantarSemaforo(idSemaforo);
        }
        /*levantarSemaforo(idSemaforo);*/
        usleep(1000 * 100);
    }
    return 0;
}
