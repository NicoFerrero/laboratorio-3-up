#ifndef _COLA_H
#define _COLA_H

#define LARGO_MENSAJE 1000

typedef struct tipo_mensaje mensaje;
struct tipo_mensaje
{
    long long_dest;
    int int_rte;
    Eventos int_evento;
    char char_mensaje[LARGO_MENSAJE];
};

int creoIdColaMensajes(int clave);
int enviarMensaje(int id_cola_mensajes, long rLongDest, int rIntRte, int rIntEvento, char *rpCharMsg);
int recibirMensaje(int id_cola_mensajes, long rLongDest, mensaje *rMsg);
void borrarColaMensajes(int id_cola_mensajes);
int borrarMensajes(int id_cola_mensajes);

#endif
