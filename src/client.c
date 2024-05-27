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


// Thread que implementa o fluxo do cliente no parque.
void *enjoy(void *arg){
    //Sua lógica aqui

    client_t *self = (client_t *)arg;

    //entrar na fila da bilheteria
    queue_enter(self);

    //esperar até ser atendido
    wait_ticket(self);
    
    //brincar até o fim das moedas
    while (self->coins > 0){
        
    }

    debug("[EXIT] - O turista saiu do parque.\n");
    pthread_exit(NULL);

    //decrementar moedas
    self->coins--;
}

// Funcao onde o cliente compra as moedas para usar os brinquedos
void buy_coins(client_t *self){
    // Sua lógica aqui
    //lógica para saber se o cliente já está dentro do parque ou fora
    //revisar quantos são comprados por vez
    self->coins = MAX_COINS + 1;
}

// Função onde o cliente espera a liberacao da bilheteria para adentrar ao parque.
void wait_ticket(client_t *self){
    // Sua lógica aqui
}

// Funcao onde o cliente entra na fila da bilheteria
void queue_enter(client_t *self){
    // Sua lógica aqui.
    debug("[WAITING] - Turista [%d] entrou na fila do portao principal\n", self->id);

    // Sua lógica aqui.
    buy_coins(self);

    // Sua lógica aqui.
    debug("[CASH] - Turista [%d] comprou [%d] moedas.\n", self->id, self->coins);
}

// Essa função recebe como argumento informações sobre o cliente e deve iniciar os clientes.
void open_gate(client_args *args){
    // Sua lógica aqui
}

// Essa função deve finalizar os clientes
void close_gate(){
   //Sua lógica aqui
}

