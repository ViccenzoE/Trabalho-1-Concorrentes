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
    // Brincar até o fim das moedas.
    while (self->coins > 0){
        // Escolher um brinquedo.
        int toy_id = rand() % num_toys;

        // Mutex compartilhado com toy.c: clientes não podem tentar entrar no brinquedo se ele estiver em operação. Os brinquedos travam esses mutexes.
        pthread_mutex_lock(&toy_lock[toy_id]);
        // Destrava para permitir que outro cliente passe e, finalmente, que o brinquedo obtenha a trava.
        pthread_mutex_unlock(&toy_lock[toy_id]);
        // Tenta obter o semáforo - um lugar no brinquedo escolhido.
        sem_wait(&sem_toys_enter[toy_id]);
        // Mutex compartilhado com toy.c: clientes não podem sair do brinquedo se ele estiver em operação. 
        pthread_mutex_lock(&toy_lock_out[toy_id]);
        // Destrava para permitir que outro cliente saia e, finalmente, que o brinquedo obtenha a trava.
        pthread_mutex_unlock(&toy_lock_out[toy_id]);
        
        // Decrementar moedas.
        self->coins--;
    }
    debug("[EXIT] - O turista saiu do parque.\n");
    // O cliente decrementa variável parque_aberto quando sai. Os brinquedos sairão do seu loop de funcionamento quando a variável chegar a zero.
    parque_aberto--;
    pthread_exit(NULL);
}

// Função onde o cliente compra as moedas para usar os brinquedos.
void buy_coins(client_t *self){
    // Número de moedas inteiro aleatório ente 1 e MAX_COINS.
    self->coins = (rand() % MAX_COINS) + 1;
}

// Função onde o cliente espera a liberação da bilheteria para adentrar ao parque.
void wait_ticket(client_t *self){    
    sem_wait(&sem_cliente_fila[(self->id - 1)]);
}

// Função onde o cliente entra na fila da bilheteria.
void queue_enter(client_t *self){
    // Função do arquivo queue.c que coloca um cliente na fila da bilheteria.
    enqueue(gate_queue, self->id);

    debug("[WAITING] - Turista [%d] entrou na fila do portao principal\n", self->id);

    // Inicia semáforo binário.
    sem_init(&sem_cliente_fila[(self->id - 1)], 0, 0);
    // Cliente espera para ser atendido nas bilheterias.
    wait_ticket(self);
    // Cliente compra as moedas.
    buy_coins(self);
    
    debug("[CASH] - Turista [%d] comprou [%d] moedas.\n", self->id, self->coins);
}

// Essa função recebe como argumento informações sobre o cliente e deve iniciar os clientes.
void open_gate(client_args *args){
    // Determina a variável global num_clients a partir dos argumentos.
    num_clients = args->n;
    // Atribui à variável parque_aberto o número total de clientes. Isto será decrementado cada vez que um cliente sai.
    parque_aberto = num_clients;
    // Aloca memória dinamicamente para os arrays de threads e semáforos.
    threads_clients = malloc(args->n * sizeof(pthread_t));
    sem_cliente_fila = malloc(args->n * sizeof(sem_t));
    
    // Cada cliente é representado por uma thread.
    for (int i = 0; i < args->n; i++){
        pthread_create(&threads_clients[i], NULL, enjoy , args->clients[i]); 
    }
}

// Essa função deve finalizar os clientes.
void close_gate(){
    // Une as threads.
    for (int i = 0; i < num_clients; i++) {
        pthread_join(threads_clients[i], NULL);
    }

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
}