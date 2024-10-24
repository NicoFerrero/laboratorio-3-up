#define CLAVE_BASE 33
#define ROJO 0
#define VERDE 1
#define LARGO_CIUDAD 50
#define LARGO_ARCHIVO 50
#define LARGO_CADENA 1000
#define NOMBRE_ARCHIVO "lote.dat"
#define CANTIDAD 10

typedef enum
{
    MSG_NADIE,
    MSG_PIENSO,
    MSG_JUGADOR
} Destinos;

typedef enum
{
    EVT_NINGUNO,
    EVT_NUMERO,
    EVT_ACERTO,
    EVT_NO_ACERTO,
    EVT_FINALIZAR
} Eventos;

typedef struct tipo_jugador tjugador;
struct tipo_jugador
{
    int nro_jugador;
    int id_colamensaje;
    int cant_jugadores;
};
