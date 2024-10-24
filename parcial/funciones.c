#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "funciones.h"
#include "archivos.h"
#include "def.h"

void devolverNumero(int desde, int hasta)
{
    int i;
    for (i = desde; i <= hasta; i++)
    {
        printf("%d\n", i);
    }
}

int numeroAleatorio(int desde, int hasta)
{
    return (rand() % (hasta - desde + 1)) + desde;
}

int numeroAleatorioNoRepetitivo(int desde, int hasta, int numbers[], int *size)
{
    int random_number;
    if (hasta - desde <= *size)
    {
        return -1;
    }
    do
    {
        random_number = numeroAleatorio(desde, hasta);
    } while (numbers[random_number]);
    numbers[random_number] = 1;
    *size = *size + 1;
    return random_number;
}

int semaforoCreado(void)
{
    struct stat buffer;
    int existe;
    FILE *archivo;
    existe = stat(FLAG_SEMAFORO, &buffer);
    if (existe == -1)
    {
        archivo = abrirArchivo(FLAG_SEMAFORO, "w");
        cerrarArchivo(archivo);
        return -1;
    }
    else
    {
        return 0;
    }
}
