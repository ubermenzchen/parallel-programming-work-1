#ifndef __CONTEXT__H__
#define __CONTEXT__H__

#include <stdbool.h>
#include <pthread.h>
#include <stdint.h>

typedef struct {
    bool is_valid;
    pthread_mutex_t *mutex, *_wait;
    pthread_cond_t *_cond;
} context_t;

typedef struct {
    uint64_t time;
    context_t *ctx;
    pthread_t id;
} _sleeper_arg_t;

context_t* context_create();
pthread_t context_set_time(context_t *ctx, uint64_t time);
void context_destroy(context_t *ctx);
void context_wait(context_t *ctx, pthread_t tid);

#endif