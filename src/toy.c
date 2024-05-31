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
toy_t **thread_to_toy = NULL;
pthread_t *threads_toys = NULL;
int num_toys = 0;
pthread_mutex_t map_lock = PTHREAD_MUTEX_INITIALIZER;

// Mapeia id de thread para id de brinquedo.
void initialize_thread_to_toy(toy_args *args, int i) {
    pthread_mutex_lock(&map_lock);
    thread_to_toy[i] = args->toys[i];
    pthread_mutex_unlock(&map_lock);
}

// Thread que o brinquedo vai usar durante toda a simulacao do sistema
void *turn_on(void *args){
    toy_t *toy = (toy_t *) args;
    pthread_t self = pthread_self();
    // toy_t *toy = NULL;
    int value;

    // Procura o brinquedo associado com esta thread.
    // pthread_mutex_lock(&map_lock);
    // for (int i = 0; i < num_toys; i++) {
    //     if (thread_to_toy[i] != NULL && pthread_equal(thread_to_toy[i]->thread, self)) {
    //         toy = thread_to_toy[i];
    //         break;
    //     }
    // }
    // pthread_mutex_unlock(&map_lock);

    if (toy != NULL) {
        // Acessa o id do brinquedo.
        debug("[ON] - O brinquedo [%d] foi ligado.\n", toy->id);
        int wait_time = 2*toy->id;
        while(TRUE) {
            // Aguarda wait_time segundos para as threads cliente escolherem brinquedos.
            sleep(wait_time);
            // Abre para um número de clientes igual à capacidade do brinquedo, ou quantos estiverem na fila, se menor.
            int num_enter = max(toy->capacity, 2*(toy->capacity)-sem_getvalue(&sem_toys[toy->id], &value));
            for (int i = 0; i < num_enter; i++) {
                sem_wait(&sem_toys_enter[toy->id]);
            }
            // Brinquedo funciona por 10 segundos.
            if (sem_getvalue(&sem_toys_enter[toy->id] < toy->capacity, &value)) {
                //lock mutex do cliente entrando no brinquedo
                sleep(2*wait_time);
            }
            // Deixa sair um número de clientes igual à quantidade que entrou.
            for (int i = 0; i < num_enter; i++) {
                sem_post(&sem_toys_enter[toy->id]);
            }
            //unlock mutex do cliente entrando no brinquedo           
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
    // Sua lógica aqui
    

    // Determina a variável global max_toys a partir dos argumentos.
    num_toys = args->n;

    // Aloca memória dinamicamente para os arrays de variáveis e semáforos.
    thread_to_toy = malloc(num_toys * sizeof(toy_t *));
    threads_toys = malloc(num_toys * sizeof(pthread_t));
    sem_toys = malloc(num_toys * sizeof(sem_t));
    sem_toys_enter = malloc(num_toys * sizeof(sem_t));
    sem_toys_leave = malloc(num_toys * sizeof(sem_t));

    if (thread_to_toy == NULL || threads_toys == NULL || sem_toys == NULL || sem_toys_enter == NULL || sem_toys_leave == NULL) {
        fprintf(stderr, "Erro: Falha ao alocar memória para arrays.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < args->n; i++) {
        // Inicia semáforo com value = 2*capacidade para a fila de espera, para cada brinquedo.
        sem_init(&sem_toys[i], 0, 2*(args->toys[i]->capacity));
        // Inicia semáforos com value = 0 para entrada e saída, para cada brinquedo.
        // A ideia é que as threads brinquedo funcionam como signaler, e as threads cliente, como waiter, para um evento (entrada e saída do brinquedo).
        sem_init(&sem_toys_enter[i], 0, args->toys[i]->capacity);
        sem_init(&sem_toys_leave[i], 0, 0);

        initialize_thread_to_toy(args, i);

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
        sem_destroy(&sem_toys[i]);
        sem_destroy(&sem_toys_enter[i]);
        sem_destroy(&sem_toys_leave[i]);
    }

    // Libera memória dos arrays e semáforos.
    free(thread_to_toy);
    thread_to_toy = NULL;

    free(threads_toys);
    threads_toys = NULL;

    free(sem_toys);
    sem_toys = NULL;

    free(sem_toys_enter);
    sem_toys_enter = NULL;

    free(sem_toys_leave);
    sem_toys_leave = NULL;

    num_toys = 0;
}