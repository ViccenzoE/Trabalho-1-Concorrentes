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

// "A lógica que autoriza o cliente a entrar no parque deve ser implementada nesse arquivo;"

pthread_mutex_t dequeue_mutex;

// Thread que implementa uma bilheteria
void *sell(void *args){

    debug("[INFO] - Bilheteria Abriu!\n");
    int id_cliente_atual;
    // cenario considerando que todos clientes entram na fila de 1 so vez

    // enquanto a fila tiver clientes, esses serao atendidos
    while (!is_queue_empty(gate_queue)) {
        // mutex para proteger a região critica
        // região critica : apenas 1 thread pode atender o cliente
        pthread_mutex_lock(&dequeue_mutex);
        id_cliente_atual = dequeue(gate_queue);
      
        // funcao no arquivo shared ?? que faz o cliente entrar no parque 
        autoriza_cliente(id_cliente_atual);
        pthread_mutex_unlock(&dequeue_mutex);
        // agora o cliente pode brincar

    }
    pthread_exit(NULL);
}

// Essa função recebe como argumento informações sobre a bilheteria e deve iniciar os atendentes.
void open_tickets(tickets_args *args){
    // Sua lógica aqui
    // cada elemento da fila(cliente) é colocado em um thread_tickets, ate que todos sejam atendidos 
    pthread_t threads_tickets[args->n];
    pthread_mutex_init(&dequeue_mutex, NULL);
    for (int i = 0; i < args->n; i++){
        pthread_create(&threads_tickets[i], NULL, sell , NULL); 
    }

    for (int j = 0; j < args->n; j++) {
        pthread_join(threads_tickets[j], NULL);
    }

}

// Essa função deve finalizar a bilheteria
void close_tickets(){
    //Sua lógica aqui
    // se alcancou numero max de clientes, encerra as threads/semaforos/mutexes

    pthread_mutex_destroy(&dequeue_mutex);
}