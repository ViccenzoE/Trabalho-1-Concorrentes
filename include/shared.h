#ifndef __SHARED_H__
#define __SHARED_H__

#include <defs.h>

// Você pode declarar novas funções (ou variaveis compartilhadas) aqui

extern int *parque_aberto = 1;
extern sem_t *sem_toys_enter;
extern pthread_mutex_t *mutex_cliente_fila;
extern pthread_mutex_t *toy_lock;

/**********************************
 *          ATENÇÃO               *
 *   NÃO EDITAR ESSAS VARIAVEIS   *
 *          ATENÇÃO               *
 *********************************/
extern Queue *gate_queue;

#endif