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

int contagem_clientes = 0;

// Thread que implementa uma bilheteria
void *sell(void *args){

    debug("[INFO] - Bilheteria Abriu!\n");

    pthread_exit(NULL);
}

// Essa função recebe como argumento informações sobre a bilheteria e deve iniciar os atendentes.
void open_tickets(tickets_args *args){
    // Sua lógica aqui
    // cada elemento da fila(cliente) é colocado em um thread_tickets, ate que todos sejam atendidos 
    
}

// Essa função deve finalizar a bilheteria
void close_tickets(){
    //Sua lógica aqui
    // se alcancou numero max de clientes, encerra as threads/semaforos/mutexes
}