#ifndef __DEFS__H__
#define __DEFS__H__

#include <stdint.h>
#include <pthread.h>

typedef enum {
    DEAD_VIRUS,
    SECRET_INSUM,
    INJECTION
} material_t;

typedef struct {
    material_t material;
    uint64_t counter;
    pthread_t *mutex;
} material_bucket_t;

typedef struct {
    material_bucket_t first;
    material_bucket_t second;
    char name[10];
} laboratory_t;

typedef struct {
    material_t have;
} infected_t;

typedef struct {
    material_t dead_virus[3];
    material_t secret_insum[3];
    material_t injection[3];
} holder_t;

#endif