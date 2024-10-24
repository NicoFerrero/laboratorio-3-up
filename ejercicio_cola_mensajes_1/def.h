#define CLAVE_BASE 33
#define ROJO 0
#define VERDE 1
#define LARGO_CIUDAD 50
#define LARGO_ARCHIVO 50
#define LARGO_CADENA 1000
#define NOMBRE_ARCHIVO "lote.dat"
#define CANTIDAD 10

typedef struct tipo_dato dato;
struct tipo_dato
{
    int numero;
    char letra;
};

typedef enum
{
    MSG_NADIE,
    MSG_SUPERMERCADO,
    MSG_REPOSITOR,
    MSG_CLIENTE
} Destinos;
