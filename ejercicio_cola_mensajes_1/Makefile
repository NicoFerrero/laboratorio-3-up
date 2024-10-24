#CC=gcc-4.1
CC=gcc

CFLAGS=-Wall -pedantic-errors -std=gnu89 -O3

HEADER=-I./

all: clean repositor supermercado

repositor:
	$(CC) -o repositor repositor.c funciones.c semaforo.c archivos.c clave.c memoria.c cola.c $(CFLAGS) $(HEADER)

supermercado:
	$(CC) -o supermercado supermercado.c funciones.c semaforo.c  archivos.c clave.c memoria.c cola.c $(CFLAGS) $(HEADER)

clean:
	rm -rf *o repositor supermercado