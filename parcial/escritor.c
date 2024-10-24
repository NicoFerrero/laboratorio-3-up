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
    char descripcion[LARGO_CADENA];
    int precio;
    char lote[LARGO_ARCHIVO];
    char cadena[LARGO_CADENA];
    idSemaforo = crearSemaforo();
    /*VERIFICO SI EL SEMAFORO YA FUE INICIADO*/
    if (semaforoCreado() == -1)
    {
        iniciarSemaforo(idSemaforo, VERDE);
        printf("INICIA SEMAFORO\n");
    }
    while (1)
    {
        /*PIDO DATOS DEL PRODUCTO*/
        printf("Ingrese la descripcion del producto: \n");
        memset(descripcion, 0x00, sizeof(descripcion));
        scanf(" %[^\n]", descripcion);
        printf("Ingrese el precio del producto: \n");
        scanf("%d", &precio);
        /*ELIJO A QUE CAJA DERIVO EL PEDIDO*/
        memset(lote, 0x00, sizeof(lote));
        if (precio < 20000)
        {
            strcpy(lote, LOTE_1);
        }
        else
        {
            strcpy(lote, LOTE_2);
        }
        /*LIMPIO Y ARMO LINEA A ESCRIBIR*/
        memset(cadena, 0x00, sizeof(cadena));
        snprintf(cadena, sizeof(cadena), "%s\t%d\n", descripcion, precio);
        /*ESPERO SEMAFORO PARA ENTRAR A ZONA CRITICA*/
        printf("ESPERANDO SEMAFORO\n");
        esperarSemaforo(idSemaforo);
        archivo = abrirArchivo(lote, "a");
        if (archivo != NULL)
        {
            /*ESCRIBO LINEA*/
            escribir(archivo, cadena);
            cerrarArchivo(archivo);
        }
        /*LIBERO SEMAFORO*/
        printf("LEVANTO SEMAFORO\n");
        levantarSemaforo(idSemaforo);
        usleep(1000 * 1000);
    }
    return 0;
}
