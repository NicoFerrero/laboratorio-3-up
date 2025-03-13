#define CLAVE_BASE 114
#define LARGO_CADENA 1000
#define LARGO_ESPIRAL 95
#define CANT_JUGADORES 2

typedef struct tipo_jugador Jugador;
struct tipo_jugador
{
    int idJugador;
    int idColaMensajes;
};

typedef struct estado_semaforo semaforo;
struct estado_semaforo
{
    int activoEspiral;
    int activoJugadores;
};

typedef enum
{
    MSG_NADIE,
    MSG_HUMO,
    MSG_JUGADOR
} Destinos;
