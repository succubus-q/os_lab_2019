#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <getopt.h>

typedef struct {
    int start;
    int end;
    int mod;
    long long *result;
    pthread_mutex_t *mutex;
} ThreadArgs;

void *compute_factorial(void *args) {
    ThreadArgs *thread_args = (ThreadArgs *)args;
    long long partial_result = 1;

    for (int i = thread_args->start; i <= thread_args->end; i++) {
        partial_result = (partial_result * i) % thread_args->mod;
    }

    // Синхронизация с использованием мьютекса
    pthread_mutex_lock(thread_args->mutex);
    *(thread_args->result) = (*(thread_args->result) * partial_result) % thread_args->mod;
    pthread_mutex_unlock(thread_args->mutex);

    return NULL;
}

int main(int argc, char **argv) {
    int k = -1;
    int pnum = -1;
    int mod = -1;

    while (1) {
        static struct option options[] = {
            {"k", required_argument, 0, 'k'},
            {"pnum", required_argument, 0, 'p'},
            {"mod", required_argument, 0, 'm'},
            {0, 0, 0, 0}
        };

        int option_index = 0;
        int c = getopt_long(argc, argv, "k:p:m:", options, &option_index);

        if (c == -1) break;

        switch (c) {
            case 'k':
                k = atoi(optarg);
                break;
            case 'p':
                pnum = atoi(optarg);
                break;
            case 'm':
                mod = atoi(optarg);
                break;
            default:
                printf("Usage: %s -k <number> --pnum=<threads> --mod=<modulus>\n", argv[0]);
                return 1;
        }
    }

    if (k <= 0 || pnum <= 0 || mod <= 0) {
        printf("All arguments (k, pnum, mod) must be positive integers.\n");
        return 1;
    }

    pthread_t threads[pnum];
    ThreadArgs thread_args[pnum];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    long long result = 1;

    int chunk_size = k / pnum;
    int remainder = k % pnum;

    int start = 1;
    for (int i = 0; i < pnum; i++) {
        int end = start + chunk_size - 1;
        if (i < remainder) end++;

        thread_args[i].start = start;
        thread_args[i].end = end;
        thread_args[i].mod = mod;
        thread_args[i].result = &result;
        thread_args[i].mutex = &mutex;

        if (pthread_create(&threads[i], NULL, compute_factorial, &thread_args[i]) != 0) {
            perror("pthread_create");
            return 1;
        }

        start = end + 1;
    }

    for (int i = 0; i < pnum; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            return 1;
        }
    }

    printf("Factorial %d! mod %d = %lld\n", k, mod, result);

    pthread_mutex_destroy(&mutex);
    return 0;
}
