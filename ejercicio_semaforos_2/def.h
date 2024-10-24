#ifndef _DEF_H
#define _DEF_H

#define CLAVE_BASE 33
#define ROJO 0
#define VERDE 1
#define LARGO_CIUDAD 50
#define LARGO_ARCHIVO 50
#define LARGO_CADENA 1000
#define LOTE_A "lote.a.dat"
#define LOTE_B "lote.b.dat"
#define LOTE_C "lote.c.dat"

typedef struct
{
    char tipoPedido;
    int precio;
    int tienePostre;
} Pedido;

#endif
