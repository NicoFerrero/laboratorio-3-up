#define CLAVE_BASE 41
#define LARGO_CADENA 1000
#define CANT_CORREDORES 10

typedef struct tipo_dato_hilo
{
    int idCorredor;
    int idColaMensajes;
    char nombre[LARGO_CADENA];
    int esMasRapido;
    int *ganador;
    int termine;
} Corredor;

typedef struct estado_semaforo
{
    int activoCarrera;
    int activoCorredor;
    int ganador;
} semaforo;

typedef enum
{
    MSG_NADIE,
    MSG_CARRERA,
    MSG_CORREDOR
} Destinos;
