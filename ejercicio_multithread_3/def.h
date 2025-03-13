#define CLAVE_BASE 59
#define LARGO_CADENA 1000
#define CANT_LUCHADORES 8

typedef struct tipo_luchador
{
    int idLuchador;
    int idColaMensajes;
    int vida;
    char arma[LARGO_CADENA];
    char nombre[LARGO_CADENA];
} Luchador;

typedef struct estado_semaforo
{
    int activo;
} semaforo;

typedef enum
{
    MSG_NADIE,
    MSG_COLISEO,
    MSG_LUCHADOR
} Destinos;
