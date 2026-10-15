#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t R1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t R2 = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barrier;

void *P1(void *arg) {
    (void)arg;
    pthread_mutex_lock(&R1);
    printf("P1: memegang R1\n");
    fflush(stdout);

    pthread_barrier_wait(&barrier);

    printf("P1: meminta R2...\n");
    fflush(stdout);
    pthread_mutex_lock(&R2);

    pthread_mutex_unlock(&R2);
    pthread_mutex_unlock(&R1);
    return NULL;
}

void *P2(void *arg) {
    (void)arg;
    pthread_mutex_lock(&R2);
    printf("P2: memegang R2\n");
    fflush(stdout);

    pthread_barrier_wait(&barrier);

    printf("P2: meminta R1...\n");
    fflush(stdout);
    pthread_mutex_lock(&R1);

    pthread_mutex_unlock(&R1);
    pthread_mutex_unlock(&R2);
    return NULL;
}

int main(void) {
    pthread_t t1, t2;
    pthread_barrier_init(&barrier, NULL, 2);

    printf("Program deadlock dimulai. PID: %d\n", getpid());

    pthread_create(&t1, NULL, P1, NULL);
    pthread_create(&t2, NULL, P2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&R1);
    pthread_mutex_destroy(&R2);
    return 0;
}
