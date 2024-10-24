#include <stdio.h>
#include <stdlib.h>
#include "sys/ipc.h"
#include "clave.h"

key_t crearClave(int base)
{
    key_t clave;
    clave = ftok("/bin/ls", base);
    if (clave == (key_t)-1)
    {
        printf("No puedo conseguir clave semaforo, mem compartida, etc.\n");
        exit(0);
    }
    return clave;
}
