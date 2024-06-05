#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <defs.h>

extern int num_clients;
extern pthread_t *threads_clients;
void queue_enter(client_t *self);

/********************************
 *          ATENÇÃO             *
 *   NÃO EDITAR ESSAS FUNÇÕES   *
 *          ATENÇÃO             *
 *******************************/
void open_gate(client_args *args);
void close_gate();

#endif