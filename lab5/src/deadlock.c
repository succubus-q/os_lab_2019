#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

void *thread_func1(void *arg) {
    pthread_mutex_lock(&mutex1);
    printf("Thread 1: locked mutex1\n");
    sleep(1); // Эмулируем длительную операцию
    
    printf("Thread 1: waiting for mutex2\n");
    pthread_mutex_lock(&mutex2); // Попытка захватить mutex2
    printf("Thread 1: locked mutex2\n");
    
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
    printf("Thread 1: released mutex1 and mutex2\n");
    return NULL;
}

void *thread_func2(void *arg) {
    pthread_mutex_lock(&mutex2);
    printf("Thread 2: locked mutex2\n");
    sleep(1); // Эмулируем длительную операцию
    
    printf("Thread 2: waiting for mutex1\n");
    pthread_mutex_lock(&mutex1); // Попытка захватить mutex1
    printf("Thread 2: locked mutex1\n");
    
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);
    printf("Thread 2: released mutex1 and mutex2\n");
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);

    pthread_create(&thread1, NULL, thread_func1, NULL);
    pthread_create(&thread2, NULL, thread_func2, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);

    return 0;
}
