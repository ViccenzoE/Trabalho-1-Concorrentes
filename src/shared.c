#include "shared.h"

#include <queue.h>


// Você pode declarar novas funções (ou variaveis compartilhadas) aqui

pthread_mutex_t *mutex_cliente_fila; // um mutex para cada cliente??

pthread_mutex_t *mutex_cliente_atendido;
pthread_mutex_t bilheteiro_sem;



/**********************************
 *          ATENÇÃO               *
 *   NÃO EDITAR ESSAS VARIAVEIS   *
 *          ATENÇÃO               *
 *********************************/
Queue *gate_queue = NULL;