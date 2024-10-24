#ifndef _SEMAFORO_H
#define _SEMAFORO_H

int crearSemaforo();
void iniciarSemaforo(int idSemaforo, int valorInicial);
void esperarSemaforo(int idSemaforo);
void levantarSemaforo(int idSemaforo);

#endif
