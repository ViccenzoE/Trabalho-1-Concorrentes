#ifndef __SHARED_H__
#define __SHARED_H__

#include <defs.h>

// Variáveis compartilhadas
extern int parque_aberto;
extern sem_t *sem_toys_enter;
extern sem_t *sem_cliente_fila;
extern pthread_mutex_t *toy_lock;
extern pthread_mutex_t *toy_lock_out;

/**********************************
 *          ATENÇÃO               *
 *   NÃO EDITAR ESSAS VARIAVEIS   *
 *          ATENÇÃO               *
 *********************************/
extern Queue *gate_queue;

#endif