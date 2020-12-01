#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "defs.h"
#include "contex.h"

int fail(const char *message) {
    fprintf(stderr, "%s\n", message);
    return EXIT_FAILURE;
}

int failerrno(const char *message) {
    fprintf(stderr, "%s: %s\n", message, strerror(errno));
    return EXIT_FAILURE;
}

int main(int argc, char **argv) {
    context_t *ctx;
    uint64_t time;
    char *end;

    errno = 0;

    if(argc != 2)
        return fail("Favor especificar o tempo de execução no primeiro argumento do programa.");

    if((time = strtoll(argv[1], &end, 10)) == 0 || argv[1] == end)
        return fail("Número inserido inválido.");
    
    if((ctx = context_create()) == NULL)
        return fail("Erro ao criar contexto.");

    //holder_t holder;
    //laboratory_t lab1, lab2, lab3;
    //infected_t infect1, infect2, infect3;

    if(!context_set_time(ctx, time))
        return fail("Erro ao setar tempo do context.");

    context_wait(ctx);

    context_destroy(ctx);

    pthread_exit(NULL);
}