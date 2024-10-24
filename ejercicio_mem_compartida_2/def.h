#define CLAVE_BASE 33
#define ROJO 0
#define VERDE 1
#define LARGO_CIUDAD 50
#define LARGO_ARCHIVO 50
#define LARGO_CADENA 1000
#define NOMBRE_ARCHIVO "libreaciones.dat"
#define CANTIDAD 10

typedef struct tipo_dato dato;
struct tipo_dato
{
    int vehiculos;
};

typedef struct estado_semaforo semaforo;
struct estado_semaforo
{
    int activo;
};
