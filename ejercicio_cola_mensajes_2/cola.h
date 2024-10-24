#ifndef _COLA_H
#define _COLA_H

#define LARGO_MENSAJE 1000

typedef enum
{
    EVT_NINGUNO,
    EVT_CONSULTA_SALDO,
    EVT_DEPOSITO,
    EVT_EXTRACCION,
    EVT_RTA_SALDO,
    EVT_RTA_SALDO_NOK,
    EVT_RTA_DEPOSITO_OK,
    EVT_RTA_DEPOSITO_NOK,
    EVT_RTA_EXTRACCION_OK,
    EVT_RTA_EXTRACCION_NOK
} Eventos;

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
