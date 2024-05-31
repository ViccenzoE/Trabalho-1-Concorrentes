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
pthread_t *threads_toys = NULL;
int num_toys = 0;
pthread_mutex_t toy_lock;

// Thread que o brinquedo vai usar durante toda a simulacao do sistema
void *turn_on(void *args){
    toy_t *toy = (toy_t *) args;
    pthread_t self = pthread_self();
    int value;

    pthread_mutex_init(&toy_lock, NULL);
    if (toy != NULL) {
        // Acessa o id do brinquedo.
        debug("[ON] - O brinquedo [%d] foi ligado.\n", toy->id);
        int wait_time = 2*toy->id;
        while(TRUE) {
            // Aguarda wait_time segundos para as threads cliente escolherem brinquedos.
            sleep(wait_time);
            // Abre para um número de clientes igual à capacidade do brinquedo.

            // Brinquedo funciona por 10 segundos.
            if (sem_getvalue(&sem_toys_enter[toy->id] < toy->capacity, &value)) {
                pthread_mutex_lock(&toy_lock);
                sleep(2*wait_time);
            }
            int num_enter = toy->capacity - sem_getvalue(&sem_toys_enter[toy->id], &value);
            // Deixa sair um número de clientes igual à quantidade que entrou.
            for (int i = 0; i < num_enter; i++) {
                sem_post(&sem_toys_enter[toy->id]);
            }
            pthread_mutex_unlock(&toy_lock);
        }


        debug("[OFF] - O brinquedo [%d] foi desligado.\n", toy->id);
    } else {
        // Erro
        fprintf(stderr, "Erro: Não foi encontrado o brinquedo para a thread %lu\n", (unsigned long)self);
    }

    pthread_exit(NULL);
}

// Essa função recebe como argumento informações e deve iniciar os brinquedos.
void open_toys(toy_args *args){

    // Determina a variável global num_toys a partir dos argumentos.
    num_toys = args->n;

    // Aloca memória dinamicamente para os arrays de variáveis e semáforos.
    threads_toys = malloc(num_toys * sizeof(pthread_t));
    sem_toys_enter = malloc(num_toys * sizeof(sem_t));

    if (threads_toys == NULL || sem_toys_enter == NULL) {
        fprintf(stderr, "Erro: Falha ao alocar memória para arrays.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < args->n; i++) {
        // Inicia semáforo com valor igual à capacidade do brinquedo para entrada, para cada brinquedo.
        sem_init(&sem_toys_enter[i], 0, args->toys[i]->capacity);

        pthread_create(&threads_toys[i], NULL, turn_on , args->toys[i]); // args->toys[i]->id

        args->toys[i]->thread = threads_toys[i];

    }
}

// Desligando os brinquedos
void close_toys(){
    // Sua lógica aqui

    // Une as threads.
    for (int i = 0; i < num_toys; i++) {
        pthread_join(threads_toys[i], NULL);
    }

    // Destrói os semáforos.
    for (int i = 0; i < num_toys; i++) {
        sem_destroy(&sem_toys_enter[i]);
    }

    // Libera memória dos arrays e semáforos.

    free(threads_toys);
    threads_toys = NULL;

    free(sem_toys_enter);
    sem_toys_enter = NULL;

    num_toys = 0;
}