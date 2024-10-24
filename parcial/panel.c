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
    char lote[LARGO_ARCHIVO];
    int idSemaforo;
    int caja;
    int precio;
    int cantidad;
    int total;
    char descripcion[LARGO_CADENA];

    /*VERIFICO QUE TENGA 2 AERGUMENTOS (NOMBRE ARCHIVO Y NUMERO DE CAJA)*/
    if (argc != 2)
    {
        printf("No ingreso la cantidad de parametros requerida. Debe pasar la cantidad de lotes\n");
        return 0;
    }
    /*VERIFICO SI LA CAJA ELEJIDA ES VALIDA*/
    caja = atoi(argv[1]);
    if (caja != 1 && caja != 2)
    {
        printf("Las cajas disponibles son la 1 o 2 \n");
        return 0;
    }
    /*ELIJO ARCHIVO DEL CUAL LEER*/
    memset(lote, 0x00, sizeof(lote));
    switch (caja)
    {
    case 1:
        strcpy(lote, LOTE_1);
        break;
    case 2:
        strcpy(lote, LOTE_2);
        break;
    default:
        break;
    }
    idSemaforo = crearSemaforo();
    /*INICIO SEMAFORO SI NO SE INICIO*/
    if (semaforoCreado() == -1)
    {
        iniciarSemaforo(idSemaforo, VERDE);
        printf("INICIA SEMAFORO\n");
    }
    /*ESPERO SEMAMFORO PARA ENTRAR A ZONA CRITICA*/
    esperarSemaforo(idSemaforo);
    archivo = abrirArchivo(lote, "r");
    if (archivo == NULL)
    {
        /*SI NO EXISTE CREO EL ARCHIVO*/
        printf("EL ARCHIVO NO EXISTE. CREANDO EL ARCHIVO VACIO...\n");
        archivo = abrirArchivo(lote, "w");
        cerrarArchivo(archivo);
    }
    levantarSemaforo(idSemaforo);

    while (1)
    {
        /*ESPERO SEMAMFORO PARA ENTRAR A ZONA CRITICA*/
        esperarSemaforo(idSemaforo);
        archivo = abrirArchivo(lote, "r");
        if (archivo != NULL)
        {
            /*ARMO LO QUE VOY A MOSTRAR POR CONSOLA*/
            /*system("clear");*/
            printf("CAJA %d\n", caja);
            printf("PRODUCTO\tPRECIO\n");
            total = 0;
            cantidad = 0;
            memset(descripcion, 0x00, sizeof(descripcion));
            while (leerArchivoCompleto(archivo, "%s\t%d\n", descripcion, &precio))
            {
                printf("%s\t\t%d\n", descripcion, precio);
                cantidad = cantidad + 1;
                total = total + precio;
            }
            /*ADEMAS DE MOSTRAR DESCRIPCION Y PRECIO MUESTRO PRODUCTOS PROCESADO POR CAJA E IMPORTE TOTAL FACTURADO*/
            printf("Cantidad total de productos precesados: %d\n", cantidad);
            printf("importe total de productos precesados: %d\n", total);
            cerrarArchivo(archivo);
        }
        /*LIBERO SEMAFORO*/
        levantarSemaforo(idSemaforo);
        usleep(1000 * 500);
    }
    return 0;
}
