CC=gcc-4.1
#C=gcc

CFLAGS=-Wall -pedantic-errors -std=gnu89 -O3 -pthread

HEADER=-I./

all: clean corredor carrera

carrera:
	$(CC) -o carrera carrera.c funciones.c semaforo.c archivos.c clave.c memoria.c cola.c thread.c global.c $(CFLAGS) $(HEADER)

corredor:
	$(CC) -o corredor corredor.c funciones.c semaforo.c archivos.c clave.c memoria.c cola.c thread.c global.c $(CFLAGS) $(HEADER)

clean:
	rm -rf *o corredor carrera