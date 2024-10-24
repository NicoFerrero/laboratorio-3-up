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

typedef struct tipo_cliente cliente;
struct tipo_cliente
{
    int id;
    int saldo;
};

typedef enum
{
    MSG_NADIE,
    MSG_BANCO,
    MSG_CAJERO
} Destinos;
