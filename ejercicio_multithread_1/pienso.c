#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <pthread.h>
#include "def.h"
#include "semaforo.h"
#include "memoria.h"
#include "funciones.h"
#include "cola.h"
#include "thread.h"
#include "global.h"

int main(int argc, char *argv[])
{
	int pienso_un_numero;
	int id_cola_mensajes;
	int numero_recibido;
	int nro_jugador;
	int done = 0;
	mensaje msg;

	srand(time(NULL));
	/*pienso el numero y lo guardo en la variable local*/
	pienso_un_numero = numeroAleatorio(1, 99);

	printf("Pense este numero %d\n", pienso_un_numero);

	id_cola_mensajes = creoIdColaMensajes(CLAVE_BASE);

	borrarMensajes(id_cola_mensajes); /*Borra todos los mensajes que haya en la cola.*/

	while (done == 0)
	{

		recibirMensaje(id_cola_mensajes, MSG_PIENSO, &msg); /*bloqueate*/
		/*Obtengo el id del jugador que me esta enviando el mensaje.*/
		nro_jugador = msg.int_rte - MSG_JUGADOR;

		switch (msg.int_evento)
		{
		case EVT_NUMERO:

			numero_recibido = atoi(msg.char_mensaje);

			if (numero_recibido == pienso_un_numero)
			{
				enviarMensaje(id_cola_mensajes, MSG_JUGADOR + nro_jugador, MSG_PIENSO, EVT_ACERTO, "FINALIZAR");
				printf("Jugador:%d ", nro_jugador);
				printf("Has ganado! con el número %d\n", numero_recibido);
				done = 1;
			}
			else
			{
				enviarMensaje(id_cola_mensajes, MSG_JUGADOR + nro_jugador, MSG_PIENSO, EVT_NO_ACERTO, "FINALIZAR");
			}

			break;
		default:
			printf("\nPienso:%d Evento sin definir\n", nro_jugador);
			break;
		}
	};
	/*borrarColaMensajes(id_cola_mensajes);*/

	return 0;
}