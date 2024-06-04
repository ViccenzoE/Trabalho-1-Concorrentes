#ifndef __TOY_H__
#define __TOY_H__

#include <defs.h>

// Você pode declarar novas funções aqui

extern int num_toys;
extern int *num_enter;
extern int *wait_time;
extern pthread_t *threads_toys;
extern int *value;

/********************************
 *          ATENÇÃO             *
 *   NÃO EDITAR ESSAS FUNÇÕES   *
 *          ATENÇÃO             *
 *******************************/
void open_toys(toy_args *args);
void close_toys();

#endif