#ifndef __TOY_H__
#define __TOY_H__

#include <defs.h>

// Você pode declarar novas funções aqui

extern int num_toys;
extern pthread_t *threads_toys;

/********************************
 *          ATENÇÃO             *
 *   NÃO EDITAR ESSAS FUNÇÕES   *
 *          ATENÇÃO             *
 *******************************/
void open_toys(toy_args *args);
void close_toys();

#endif