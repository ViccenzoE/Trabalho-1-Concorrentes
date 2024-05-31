/**
 * Esse arquivo tem como objetivo a implementação do cliente em um parque.
 * Você pode criar novas funções dentro desse arquivo caso julgue necessário.
 * ATENÇÃO: NÃO APAGUE OU EDITE O NOME OU O(S) ARGUMENTO(S) DAS FUNÇÕES FORNECIDAS
*/

#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "client.h"
#include "queue.h"
#include "shared.h"
#include "toy.h"
#include "tickets.h"

// Thread que implementa o fluxo do cliente no parque.
void *enjoy(void *arg){
    client_t *self = (client_t *)arg;

    // Entrar na fila da bilheteria
    pthread_mutex_init(&mutex_cliente_fila[self->id], NULL);
    queue_enter(self);

    // Esperar até ser atendido
    wait_ticket(self);
    
    // Brincar até o fim das moedas
    while (self->coins > 0){
        // Escolher um brinquedo
        int toy_id = rand() % num_toys;
        
        // Esperar a vez para entrar no brinquedo
        sem_wait(&sem_toys[toy_id]);
        sem_post(&sem_toys_enter[toy_id]);
        
        // Esperar até sair do brinquedo
        sem_wait(&sem_toys_leave[toy_id]);
        
        // Decrementar moedas
        self->coins--;
    }

    debug("[EXIT] - O turista saiu do parque.\n");
    pthread_exit(NULL);
}

// Função onde o cliente compra as moedas para usar os brinquedos
void buy_coins(client_t *self){
    self->coins = MAX_COINS;
}

// Função onde o cliente espera a liberação da bilheteria para adentrar ao parque.
void wait_ticket(client_t *self){

}

// Função onde o cliente entra na fila da bilheteria
void queue_enter(client_t *self){
    debug("[WAITING] - Turista [%d] entrou na fila do portao principal\n", self->id);
    pthread_mutex_lock(&mutex_cliente_fila[self->id]);
    buy_coins(self);
    debug("[CASH] - Turista [%d] comprou [%d] moedas.\n", self->id, self->coins);
}

// Essa função recebe como argumento informações sobre o cliente e deve iniciar os clientes.
void open_gate(client_args *args){
    initialize_shared(args);

    pthread_t *threads_clients = malloc(args->n * sizeof(pthread_t));
    for (int i = 0; i < args->n; i++){
        
    }

    free(threads_clients);
}

// Essa função deve finalizar os clientes
void close_gate(){
    finalize_shared();
}