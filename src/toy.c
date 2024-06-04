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

// Inicializa variáveis globais.
int num_toys = 0;
int *num_enter = NULL;
int *wait_time = NULL;
int *value = NULL;
pthread_t *threads_toys = NULL;

// Thread que o brinquedo vai usar durante toda a simulacao do sistema
void *turn_on(void *args){
    debug("[EXCLUIR] - O brinquedo foi ACIONADO.\n");
    toy_t *toy = (toy_t *) args;
    pthread_t self = pthread_self();
    // int value;
    pthread_mutex_init(&toy_lock[(toy->id -1)], NULL);
    pthread_mutex_init(&toy_lock_out[(toy->id -1)], NULL);
    if (toy != NULL) {
        // Acessa o id do brinquedo.
        debug("[ON] - O brinquedo [%d] foi ligado.\n", (toy->id));
        // Tempo de espera do brinquedo depende da sua id, para que as transições não ocorram todas ao mesmo tempo.
        wait_time[(toy->id -1)] = 2*(toy->id -1) + 3;
        // Loop do brinquedo roda enquanto algum cliente tiver ao menos uma moeda.
        while(parque_aberto) {
            // Aguarda wait_time segundos para as threads cliente escolherem brinquedos.
            
            sleep(wait_time[(toy->id -1)]);
            debug("[EXCLUIR] - NUM_ENTER pre brincar [%d] foi ligado brinquedo [%d].\n", num_enter[(toy->id - 1)], (toy->id));
            // Impede os clientes de tentarem pegar o semáforo para entrar no brinquedo, brinquedo tentará começar a funcionar.
            pthread_mutex_lock(&toy_lock[(toy->id -1)]);
            num_enter[(toy->id -1)] = 0;
            // if (sem_getvalue(&sem_toys_enter[(toy->id -1)] < toy->capacity, &value)) {
            sem_getvalue(&sem_toys_enter[(toy->id -1)], &value[(toy->id - 1)]);
            if (value[(toy->id - 1)] < toy->capacity) {
                num_enter[(toy->id -1)] = toy->capacity - value[(toy->id - 1)];
                // Brinquedo entra em funcionamento por wait_time segundos.
                sleep(wait_time[(toy->id -1)]);
                // Abre o semáforo para num_enter clientes.
                for (int i = 0; i < num_enter[(toy->id -1)]; i++) {
                    sem_post(&sem_toys_enter[(toy->id -1)]);
                }
                debug("[EXCLUIR] - NUM_ENTER pos brincar [%d] foi ligado brinquedo [%d] value [%d].\n", num_enter[(toy->id -1)], (toy->id), value[(toy->id - 1)]);
            }
            
            // Retorna o semáforo ao seu valor inicial, com um número de operações post igual à quantidade de clientes que entrou.
//            for (int i = 0; i < num_enter[(toy->id -1)]; i++) {
//                sem_post(&sem_toys_enter[(toy->id -1)]);
//            }
            num_enter[(toy->id -1)]= 0;
            // Libera os clientes para pegarem o semáforo para entrar no brinquedo.
            pthread_mutex_unlock(&toy_lock[(toy->id -1)]);
        }
        debug("[OFF] - O brinquedo [%d] foi desligado.\n", (toy->id));
    } else {
        // Erro, o brinquedo não foi encontrado.
        fprintf(stderr, "Erro: Não foi encontrado o brinquedo para a thread %lu\n", (unsigned long)self);
    }
    pthread_exit(NULL);
}

// Essa função recebe como argumento informações e deve iniciar os brinquedos.
void open_toys(toy_args *args){

    // Determina a variável global num_toys a partir dos argumentos.
    num_toys = args->n;

    // Aloca memória dinamicamente para os arrays de threads, semáforos e mutexes.
    threads_toys = malloc(num_toys * sizeof(pthread_t));
    sem_toys_enter = malloc(num_toys * sizeof(sem_t));
    toy_lock = malloc(num_toys * sizeof(pthread_mutex_t));

    // Aloca memória dinamicamente para arrays de variáveis.
    wait_time = malloc(num_toys * sizeof(int));
    num_enter = malloc(num_toys * sizeof(int));
    value = malloc(num_toys * sizeof(int));

    // Checa se a alocação de memória para os arrays teve sucesso.
    if (threads_toys == NULL || sem_toys_enter == NULL || toy_lock == NULL) {
        fprintf(stderr, "Erro: Falha ao alocar memória para arrays.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < args->n; i++) {
        // Inicia semáforo com valor igual à capacidade do brinquedo para entrada, para cada brinquedo.
        sem_init(&sem_toys_enter[i], 0, args->toys[i]->capacity);
        // Cria as threads brinquedo.
        pthread_create(&threads_toys[i], NULL, turn_on , args->toys[i]);
        // Atualiza com a thread correspondente a cada brinquedo.
        args->toys[i]->thread = threads_toys[i];
    }
}

// Desligando os brinquedos
void close_toys(){

    // Une as threads.
    for (int i = 0; i < num_toys; i++) {
        pthread_join(threads_toys[i], NULL);
    }

    // Destrói os semáforos.
    for (int i = 0; i < num_toys; i++) {
        sem_destroy(&sem_toys_enter[i]);
    }

    // Destrói os mutexes.
    for (int i = 0; i < num_toys; i++) {
        pthread_mutex_destroy(&toy_lock[i]);
    }

    // Libera memória dos arrays.
    free(toy_lock);
    toy_lock = NULL;

    free(threads_toys);
    threads_toys = NULL;

    free(sem_toys_enter);
    sem_toys_enter = NULL;

    free(wait_time);
    wait_time = 0;

    free(num_enter);
    num_enter = 0;

    num_toys = 0;
}