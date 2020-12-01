#include <stdlib.h>
#include <unistd.h>

#ifdef DEBUG
#include <stdio.h>
#endif

#include "contex.h"

void* _sleeper(void *arg);

/*
 * Cria um contexto. Caso ocorra algum erro, a função retorna NULL.
 */
context_t* context_create() {
    context_t *ctx = malloc(sizeof(context_t));
    pthread_mutex_t *mutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_t *_wait = malloc(sizeof(pthread_mutex_t));
    pthread_cond_t *_cond = malloc(sizeof(pthread_cond_t));

    if(pthread_mutex_init(mutex, NULL) != 0)
        return NULL;
    if(pthread_mutex_init(_wait, NULL) != 0)
        return NULL;
    if(pthread_cond_init(_cond, NULL) != 0)
        return NULL;

    ctx->mutex = mutex;
    ctx->_wait = _wait;
    ctx->_cond = _cond;
    ctx->is_valid = true;

    pthread_mutex_unlock(ctx->mutex);
    pthread_mutex_lock(ctx->_wait);

    return ctx;
}

/*
 * Cria um temporizador para o contexto.
 * É criada uma nova thread que irá dormir até que se expire seu tempo dormindo.
 * Após o tempo expirar, o contexto se tornará inválido/falso.
 */
pthread_t context_set_time(context_t *ctx, uint64_t time) {
    pthread_t sleeper;
    _sleeper_arg_t *arg = malloc(sizeof(_sleeper_arg_t));

    arg->ctx = ctx;
    arg->time = time;
    arg->id = sleeper;

    if(pthread_create(&sleeper, NULL, _sleeper, (void*)arg))
        return 0;

    return sleeper;
}

/*
 * Destroi um contexto.
 */
void context_destroy(context_t *ctx) {
    pthread_mutex_destroy(ctx->mutex);
    pthread_mutex_destroy(ctx->_wait);
    pthread_cond_destroy(ctx->_cond);
    free(ctx->mutex);
    free(ctx->_wait);
    free(ctx->_cond);
    free(ctx);
}

/*
 * Aguarda o termino de um contexto, ou seja, quando o contexto não for válido (ctx->is_valid = false)
 */
void context_wait(context_t *ctx, pthread_t tid) {
    pthread_cond_wait(ctx->_cond, ctx->_wait);
    pthread_join(tid, NULL);
}

/*
 * Função auxiliar que será a thread dormindo para temporizar um contexto.
 */
void* _sleeper(void *arg) {
    _sleeper_arg_t *args = (_sleeper_arg_t*)arg;
    context_t *ctx = args->ctx;

#ifdef DEBUG
    printf("Sleeper thread id %ld started with time of %lu seconds\n", args->id, args->time);
#endif

    sleep(args->time);

#ifdef DEBUG
    printf("Sleeper thread id %ld with time of %ld seconds has expired\n", args->id, args->time);
#endif

    pthread_mutex_lock(ctx->mutex);
    ctx->is_valid = false;
    pthread_mutex_unlock(ctx->mutex);

    free(arg);

#ifdef DEBUG
    printf("Sleeper thread id %ld ended\n", args->id);
#endif

    pthread_cond_broadcast(ctx->_cond);
    pthread_exit(NULL);
}