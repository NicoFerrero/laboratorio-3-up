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
    int opcion;
    int panel;
    char mensaje[LARGO_CADENA];
    char lote[LARGO_ARCHIVO];
    idSemaforo = crearSemaforo();
    iniciarSemaforo(idSemaforo, VERDE);

    while (1)
    {
        /* PIDO DATOS DEL PEDIDO */
        printf("ESPERANDO SEMAFORO\n");
        /*esperarSemaforo(idSemaforo);*/
        printf("MENU CCI\n");
        printf("1 - LEER PANEL\n");
        printf("2 - ESCRIBIR PANEL\n");
        scanf("%d", &opcion);
        while (opcion != 1 && opcion != 2)
        {
            printf("Respuesta erronea. Elija una opcion valida\n: \n");
            printf("1 - LEER PANEL\n");
            printf("2 - ESCRIBIR PANEL\n");
            scanf("%d", &opcion);
        }
        switch (opcion)
        {
        case 1:
            printf("DE QUE PANEL QUIERE VER EL MENSAJE? (1, 2 o 3)\n");
            scanf("%d", &panel);
            while (panel != 1 && panel != 2 && panel != 3)
            {
                printf("Respuesta erronea. Elija un panel valido\n: \n");
                scanf("%d", &panel);
            }
            sprintf(lote, "lote.%d.dat", panel);
            esperarSemaforo(idSemaforo);
            archivo = abrirArchivo(lote, "r");
            if (archivo != NULL)
            {
                if (leerArchivoCompleto(archivo, "%[^\n]", mensaje))
                {
                    printf("EL MENSAJE DEL PANEL %d ES: %s\n", panel, mensaje);
                }
                cerrarArchivo(archivo);
                levantarSemaforo(idSemaforo);
            }
            else
            {
                esperarSemaforo(idSemaforo);
                archivo = abrirArchivo(lote, "w");
                if (archivo != NULL)
                {
                    printf("EL PANEL NO TIENE UN MENSAJE. GRABANDO MENSAJE DEFAULT...\n");
                    escribir(archivo, "default\n");
                    cerrarArchivo(archivo);
                    levantarSemaforo(idSemaforo);
                }
            }
            break;
        case 2:
            printf("EN QUE PANEL QUIERE ESCRIBIR UN MENSAJE? (1, 2 o 3)\n");
            scanf("%d", &panel);
            while (opcion != 1 && opcion != 2)
            {
                printf("Respuesta erronea. Elija un panel valido\n: \n");
                scanf("%d", &panel);
            }
            printf("INGRESE SU MENSAJE: ");
            /* while (getchar() != '\n')*/
            ;
            /* fgets(mensaje, sizeof(mensaje), stdin);*/
            scanf(" %[^\n]", mensaje);
            memset(lote, 0x00, sizeof(lote));
            sprintf(lote, "lote.%d.dat", panel);
            esperarSemaforo(idSemaforo);
            archivo = abrirArchivo(lote, "w");
            if (archivo != NULL)
            {
                escribir(archivo, mensaje);
                cerrarArchivo(archivo);
                levantarSemaforo(idSemaforo);
            }
            else
            {
                printf("chau\n");
                continue;
            }
            break;
        default:
            break;
        }
        printf("LEVANTO SEMAFORO\n");
        /*levantarSemaforo(idSemaforo);*/
        usleep(1000 * 1000);
    }
    return 0;
}
