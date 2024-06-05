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

//Inicializa variáveis globais
pthread_mutex_t dequeue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t *threads_tickets = NULL;
int num_tickets = 0;

// Thread que implementa uma bilheteria
void *sell(void *args){
    debug("[INFO] - Bilheteria Abriu!\n");
    // Variável para guardar o cliente que está sendo atendido
    int id_cliente_atual;
    // Enquanto a fila tiver clientes, eles serão atendidos
    while (!is_queue_empty(gate_queue)) {
        // Mutex para proteger a região crítica: apenas 1 thread pode atender o cliente X
        pthread_mutex_lock(&dequeue_mutex);
        // Um cliente é atendido pelo atendente
        id_cliente_atual = dequeue(gate_queue);
        // Libera o cliente para entrar no parque
        sem_post(&sem_cliente_fila[(id_cliente_atual - 1)]);
        // Fim da região crítica
        pthread_mutex_unlock(&dequeue_mutex);
        // Agora o cliente pode brincar
    }
    // A thread atendente terminou sua função
    pthread_exit(NULL);
}

// Essa função recebe como argumento informações sobre a bilheteria e deve iniciar os atendentes.
void open_tickets(tickets_args *args){

    num_tickets = args ->n;
    threads_tickets = malloc(num_tickets * sizeof(pthread_t));
    // Inicia o mutex que protege uma região crítica
    pthread_mutex_init(&dequeue_mutex, NULL);

    for (int i = 0; i < num_tickets; i++){
        // Cada funcionário é representado por uma thread
        pthread_create(&threads_tickets[i], NULL, sell , args->tickets[i]); 
    }

    // Aguarda todas as threads tickets terminarem
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