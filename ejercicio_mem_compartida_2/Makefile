#CC=gcc-4.1
CC=gcc

CFLAGS=-Wall -pedantic-errors -std=gnu89 -O3

HEADER=-I./

all: clean auto peaje

auto:
	$(CC) -o auto auto.c funciones.c semaforo.c archivos.c clave.c memoria.c $(CFLAGS) $(HEADER)

peaje:
	$(CC) -o peaje peaje.c funciones.c semaforo.c  archivos.c clave.c memoria.c $(CFLAGS) $(HEADER)

clean:
	rm -rf *o *dat auto peaje