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

// Inicializa variáveis globais.
int num_clients = 0;
pthread_t *threads_clients = NULL;

// Thread que implementa o fluxo do cliente no parque.
void *enjoy(void *arg){
    client_t *self = (client_t *)arg;
    queue_enter(self);
    // Brincar até o fim das moedas
    while (self->coins > 0){
        // Escolher um brinquedo
        int toy_id = rand() % num_toys;

        // Clientes não podem tentar entrar no brinquedo se ele estiver funcionando.
        pthread_mutex_lock(&toy_lock[toy_id]);
        pthread_mutex_unlock(&toy_lock[toy_id]);
        // Esperar a vez para entrar no brinquedo
        sem_wait(&sem_toys_enter[toy_id]);

        
        // Decrementar moedas
        self->coins--;
    }

    debug("[EXIT] - O turista saiu do parque.\n");
    pthread_exit(NULL);
}

// Função onde o cliente compra as moedas para usar os brinquedos
void buy_coins(client_t *self){
    self->coins = (rand() % MAX_COINS) + 1;
}

// Função onde o cliente espera a liberação da bilheteria para adentrar ao parque.
void wait_ticket(client_t *self){    
    sem_wait(&sem_cliente_fila[self->id]);
}

// Função onde o cliente entra na fila da bilheteria
void queue_enter(client_t *self){

    enqueue(gate_queue, self->id);

    // Sua lógica aqui.
    debug("[WAITING] - Turista [%d] entrou na fila do portao principal\n", self->id);

    //semaforo binario 
    sem_init(&sem_cliente_fila[self->id], 0, 0);
    wait_ticket(self);
    
    buy_coins(self);
    // Sua lógica aqui.
    
    debug("[CASH] - Turista [%d] comprou [%d] moedas.\n", self->id, self->coins);
}

// Essa função recebe como argumento informações sobre o cliente e deve iniciar os clientes.
void open_gate(client_args *args){
    //initialize_shared(args);
    threads_clients = malloc(args->n * sizeof(pthread_t));
    sem_cliente_fila = malloc(args->n * sizeof(sem_t));
    
    for (int i = 0; i < args->n; i++){
        pthread_create(&threads_clients[i], NULL, enjoy , args->clients[i]); 
        // queue_enter(args->clients[i]);
    }

    // Une as threads.
    for (int i = 0; i < num_clients; i++) {
        pthread_join(threads_clients[i], NULL);
    }
}

// Essa função deve finalizar os clientes
void close_gate(){
    
    // Destrói os mutexes.
    for (int i = 0; i < num_clients; i++) {
        sem_destroy(&sem_cliente_fila[i]);
    }

    // Libera memória dos arrays.
    free(threads_clients);
    threads_clients = NULL;

    free(sem_cliente_fila);
    sem_cliente_fila = NULL;

    num_clients = 0;

    //finalize_shared();
}