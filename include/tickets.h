#ifndef __TICKETS_H__
#define __TICKETS_H__

#include <defs.h>

extern pthread_mutex_t dequeue_mutex;
extern pthread_t *threads_tickets;
extern int num_tickets;

/********************************
 *          ATENÇÃO             *
 *   NÃO EDITAR ESSAS FUNÇÕES   *
 *          ATENÇÃO             *
 *******************************/
void open_tickets(tickets_args *args);
void close_tickets();

#endif