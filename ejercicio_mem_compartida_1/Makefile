#CC=gcc-4.1
CC=gcc

CFLAGS=-Wall -pedantic-errors -std=gnu89 -O3

HEADER=-I./

all: clean pienso jugador

pienso:
	$(CC) -o pienso pienso.c funciones.c semaforo.c archivos.c clave.c memoria.c $(CFLAGS) $(HEADER)

jugador:
	$(CC) -o jugador jugador.c funciones.c semaforo.c  archivos.c clave.c memoria.c $(CFLAGS) $(HEADER)

clean:
	rm -rf *o pienso jugador