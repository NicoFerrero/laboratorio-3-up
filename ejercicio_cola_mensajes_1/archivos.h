#ifndef _ARCHIVOS_H
#define _ARCHIVOS_H

FILE *abrirArchivo(char archivo[], char modo[]);
void cerrarArchivo(FILE *archivo);
void escribir(FILE *archivo, char linea[]);
int leerArchivoCompleto(FILE *archivo, char *formato, ...);
/*void leerLineaArchivo(FILE *archivo, int numeroLinea);
void leerArchivoDesdeHasta(FILE *archivo, int lineaInicio, int lineaFin);*/
void leer(FILE *archivo);
int renombrarArchivo(char nuevoNombre[], char viejoNombre[]);

#endif
