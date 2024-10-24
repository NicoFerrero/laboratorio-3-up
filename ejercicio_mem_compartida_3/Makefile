#CC=gcc-4.1
CC=gcc

CFLAGS=-Wall -pedantic-errors -std=gnu89 -O3

HEADER=-I./

all: clean colchoneria vendedor

colchoneria:
	$(CC) -o colchoneria colchoneria.c funciones.c semaforo.c archivos.c clave.c memoria.c $(CFLAGS) $(HEADER)

vendedor:
	$(CC) -o vendedor vendedor.c funciones.c semaforo.c  archivos.c clave.c memoria.c $(CFLAGS) $(HEADER)

clean:
	rm -rf *o *dat colchoneria vendedor