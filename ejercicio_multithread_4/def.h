#define CLAVE_BASE 63
#define LARGO_CADENA 1000
#define CANT_CORREDORES 2

typedef struct tipo_dato_hilo
{
    int idCorredor;
    int idColaMensajes;
} DatoHilo;

typedef struct estado_semaforo
{
    int activoCarrera;
    int activoCorredor;
    int comenzo;
} semaforo;

typedef enum
{
    MSG_NADIE,
    MSG_CARRERA,
    MSG_CORREDOR
} Destinos;
