CC=gcc-4.1
#CC=gcc

CFLAGS=-Wall -pedantic-errors -std=gnu89 -O3

HEADER=-I./

all: clean escritor panel

escritor:
	$(CC) -o escritor escritor.c archivos.c semaforo.c clave.c funciones.c $(CFLAGS) $(HEADER)

panel:
	$(CC) -o panel panel.c archivos.c semaforo.c clave.c funciones.c $(CFLAGS) $(HEADER)

clean:
	rm -rf *.o *.dat panel escritor