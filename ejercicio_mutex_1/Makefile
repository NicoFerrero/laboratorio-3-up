#CC=gcc-4.1
CC=gcc

CFLAGS=-Wall -pedantic-errors -std=gnu89 -O3 -pthread

HEADER=-I./

all: clean adivinar

adivinar:
	$(CC) -o adivinar adivinar.c funciones.c semaforo.c archivos.c clave.c memoria.c cola.c thread.c global.c $(CFLAGS) $(HEADER)

clean:
	rm -rf *o adivinar