#define CLAVE_BASE 116
#define ROJO 0
#define VERDE 1
#define LARGO_CIUDAD 50
#define LARGO_ARCHIVO 50
#define LARGO_CADENA 1000
#define NOMBRE_ARCHIVO "lote.dat"
#define CANTIDAD 10

typedef enum
{
    NADIE,
    CONEJO,
    GATO,
    PERRO
} Animal;

typedef struct tipo_jugador Jugador;
struct tipo_jugador
{
    Animal tipo;
    int turnos;
    int pasos;
    int idColaMensajes;
    int *ganador;
};

typedef struct estado_semaforo semaforo;
struct estado_semaforo
{
    int activo;
};

typedef enum
{
    MSG_NADIE,
    MSG_CONEJO,
    MSG_GATO,
    MSG_PERRO,
    MSG_PISTA
} Destinos;
