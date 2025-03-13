#define CLAVE_BASE 81
#define LARGO_CADENA 1000
#define CANT_PATEADORES 5

typedef struct tipo_pateador
{
    int idPateador;
    int idColaMensajes;
    char nombre[LARGO_CADENA];
    int eventoTiro;
} Pateador;

typedef struct estado_semaforo
{
    int activoArquero;
    int activoPateador;
    int comenzo;
} semaforo;

typedef enum
{
    MSG_NADIE,
    MSG_ARQUERO,
    MSG_PATEADOR
} Destinos;
