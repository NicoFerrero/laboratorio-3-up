#define CLAVE_BASE 45
#define LARGO_CADENA 1000
#define CANT_JUGADORES 2

typedef struct tipo_jugador
{
    int idJugador;
    int idColaMensajes;
} Jugador;

typedef struct estado_semaforo
{
    int activo;
} semaforo;

typedef enum
{
    MSG_NADIE,
    MSG_BOLERA,
    MSG_JUGADOR
} Destinos;
