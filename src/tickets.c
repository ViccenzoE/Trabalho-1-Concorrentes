/**
 * Esse arquivo tem como objetivo a implementação da bilheteria em um parque.
 * Você pode criar novas funções dentro desse arquivo caso julgue necessário.
 * ATENÇÃO: NÃO APAGUE OU EDITE O NOME OU O(S) ARGUMENTO(S) DAS FUNÇÕES FORNECIDAS.
*/
#include <stdio.h>
#include <stdlib.h>
#include <tickets.h>
#include <unistd.h>

#include <queue.h>
#include "shared.h"

pthread_mutex_t dequeue_mutex;
pthread_t *threads_tickets = NULL;
int num_tickets = 0;

// Thread que implementa uma bilheteria
void *sell(void *args){
    debug("[INFO] - Bilheteria Abriu!\n");
    int id_cliente_atual;
    // enquanto a fila tiver clientes, eles serão atendidos
    while (!is_queue_empty(gate_queue)) {
        // mutex para proteger a região crítica: apenas 1 thread pode atender o cliente X
        pthread_mutex_lock(&dequeue_mutex);
        // um cliente é atendido pelo atendente
        id_cliente_atual = dequeue(gate_queue);
        // libera o cliente para entrar no parque
        sem_post(&sem_cliente_fila[id_cliente_atual]);
        // fim da região crítica
        pthread_mutex_unlock(&dequeue_mutex);
        // agora o cliente pode brincar
    }
    // a thread atendente terminou sua função
    pthread_exit(NULL);
}

// Essa função recebe como argumento informações sobre a bilheteria e deve iniciar os atendentes.
void open_tickets(tickets_args *args){

    num_tickets = args ->n;
    threads_tickets = malloc(num_tickets * sizeof(pthread_t));
    pthread_mutex_init(&dequeue_mutex, NULL);

    for (int i = 0; i < num_tickets; i++){
        // cada cliente da fila é atendido em um thread_tickets, até que todos sejam atendidos 
        pthread_create(&threads_tickets[i], NULL, sell , NULL); 
    }

    // aguarda todas as threads tickets terminarem
    for (int j = 0; j < num_tickets; j++) {
        pthread_join(threads_tickets[j], NULL);
    }
}

// Essa função deve finalizar a bilheteria
void close_tickets(){

    free(threads_tickets);
    threads_tickets = NULL;
    pthread_mutex_destroy(&dequeue_mutex);
}