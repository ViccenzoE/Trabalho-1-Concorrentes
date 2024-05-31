#ifndef __TOY_H__
#define __TOY_H__

#include <defs.h>

// Você pode declarar novas funções aqui

extern int num_toys;
extern toy_t **thread_to_toy;
extern sem_t *sem_toys;
extern sem_t *sem_toys_enter;
extern sem_t *sem_toys_leave;
extern pthread_mutex_t map_lock;

void initialize_thread_to_toy(toy_args *args, int i);

/********************************
 *          ATENÇÃO             *
 *   NÃO EDITAR ESSAS FUNÇÕES   *
 *          ATENÇÃO             *
 *******************************/
void open_toys(toy_args *args);
void close_toys();

#endif