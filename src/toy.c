/**
 * Esse arquivo tem como objetivo a implementação de um brinquedo em um parque.
 * Você pode criar novas funções dentro desse arquivo caso julgue necessário.
 * ATENÇÃO: NÃO APAGUE OU EDITE O NOME OU O(S) ARGUMENTO(S) DAS FUNÇÕES FORNECIDAS
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "toy.h"
#include "shared.h"


// Thread que o brinquedo vai usar durante toda a simulacao do sistema
void *turn_on(void *args){

    debug("[ON] - O brinquedo  [%d] foi ligado.\n", rand()); // Altere para o id do brinquedo

    debug("[OFF] - O brinquedo [%d] foi desligado.\n", rand()); // Altere para o id do brinquedo

    pthread_exit(NULL);
}


// Essa função recebe como argumento informações e deve iniciar os brinquedos.
void open_toys(toy_args *args){
    // Sua lógica aqui
    sem_t sem_toys[args->n];
    for (int i = 0 ; i < args->n; i++ ) {
        // inicia um semaforo. com value = capacidade, para cada brinquedo
        sem_init(&sem_toys[i], 0, args->toys[i]->capacity);
    }
}

// Desligando os brinquedos
void close_toys(){
    // Sua lógica aqui
    //passo 1 - desalocar os semaforos criados
}

//Teste push - Bruno
