#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "def.h"
#include "archivos.h"
#include "semaforo.h"
#include "funciones.h"

int main(int argc, char *argv[])
{
    FILE *archivo;
    int idSemaforo;
    Pedido pedido;
    int postre;
    int aleatorio;
    char cadena[LARGO_CADENA];

    idSemaforo = crearSemaforo();

    while (1)
    {
        /* PIDO DATOS DEL PEDIDO */
        printf("ESPERANDO SEMAFORO\n");
        /*esperarSemaforo(idSemaforo);*/
        memset(&pedido, 0, sizeof(pedido));
        aleatorio = numeroAleatorio(1, 3);
        switch (aleatorio)
        {
        case 1:
            pedido.precio = 1000;
            pedido.tipoPedido = 'a';
            break;
        case 2:
            pedido.precio = 2000;
            pedido.tipoPedido = 'b';
            break;
        case 3:
            pedido.precio = 3000;
            pedido.tipoPedido = 'c';
            break;
        default:
            break;
        }
        printf("Usted pidio el menu %c con un precio de %d\n", pedido.tipoPedido, pedido.precio);
        printf("Quiere prostre en su pedido?: \n");
        printf("0 - NO: \n");
        printf("1 - SI: \n");
        scanf("%d", &postre);
        while (postre != 0 && postre != 1)
        {
            printf("Respuesta erronea. Quiere postre en su pedido?: \n");
            printf("0 - NO: \n");
            printf("1 - SI: \n");
            scanf("%d", &postre);
        }
        pedido.tienePostre = postre;
        if (pedido.tienePostre)
        {
            pedido.precio = pedido.precio + 200;
        }
        /* Armo la linea a escribir en el archivo */
        memset(cadena, 0x00, sizeof(cadena));
        snprintf(cadena, sizeof(cadena), "%c\t%01d\t%04d\n", pedido.tipoPedido, pedido.tienePostre, pedido.precio);
        esperarSemaforo(idSemaforo);
        switch (pedido.tipoPedido)
        {
        case 'a':
            archivo = abrirArchivo(LOTE_A, "a");
            break;
        case 'b':
            archivo = abrirArchivo(LOTE_B, "a");
            break;
        case 'c':
            archivo = abrirArchivo(LOTE_C, "a");
            break;
        default:
            break;
        }
        if (archivo != NULL)
        {
            escribir(archivo, cadena);
            cerrarArchivo(archivo);
            levantarSemaforo(idSemaforo);
        }
        else
        {
            continue;
        }
        printf("LEVANTO SEMAFORO\n");
        /*levantarSemaforo(idSemaforo);*/
        usleep(1000 * 1000);
    }
    return 0;
}
