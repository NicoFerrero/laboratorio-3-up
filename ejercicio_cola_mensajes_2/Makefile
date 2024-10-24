#CC=gcc-4.1
CC=gcc

CFLAGS=-Wall -pedantic-errors -std=gnu89 -O3

HEADER=-I./

all: clean cajero banco

cajero:
	$(CC) -o cajero cajero.c funciones.c semaforo.c archivos.c clave.c memoria.c cola.c $(CFLAGS) $(HEADER)

banco:
	$(CC) -o banco banco.c funciones.c semaforo.c  archivos.c clave.c memoria.c cola.c $(CFLAGS) $(HEADER)

clean:
	rm -rf *o cajero banco