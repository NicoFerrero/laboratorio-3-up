#CC=gcc-4.1
C=gcc

CFLAGS=-Wall -pedantic-errors -std=gnu89 -O3 -pthread

HEADER=-I./

all: clean luchador coliseo

coliseo:
	$(CC) -o coliseo coliseo.c funciones.c semaforo.c archivos.c clave.c memoria.c cola.c thread.c global.c $(CFLAGS) $(HEADER)

luchador:
	$(CC) -o luchador luchador.c funciones.c semaforo.c archivos.c clave.c memoria.c cola.c thread.c global.c $(CFLAGS) $(HEADER)

clean:
	rm -rf *o luchador coliseo