#include "shared.h"

#include <queue.h>


// Você pode declarar novas funções (ou variaveis compartilhadas) aqui



//pthread_mutex_t *mutex_cliente_atendido;
//pthread_mutex_t bilheteiro_sem;

int parque_aberto = 1;
sem_t *sem_cliente_fila = NULL;
sem_t *sem_toys_enter = NULL;
pthread_mutex_t *toy_lock = NULL;
pthread_mutex_t *toy_lock_out = NULL;

/**********************************
 *          ATENÇÃO               *
 *   NÃO EDITAR ESSAS VARIAVEIS   *
 *          ATENÇÃO               *
 *********************************/
Queue *gate_queue = NULL;