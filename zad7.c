#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#define SAMPLE_SIZE 1000000     // liczba losowań punktu
#define NUM_THREADS 100         // liczba wątków

void *approxPi(void *_tid){
    int i;
    long circleCount = 0;
    long tid = (long)_tid;
    double x, y, norm;

    printf("Wątek %ld rozpoczyna pracę.\n",tid);

    for(i = 0; i < SAMPLE_SIZE; i++){
        x = (double)rand()/RAND_MAX * 2.0 - 1.0;    // losowa liczba w zakresie od -1 do 1 (współrzędna x)
        y = (double)rand()/RAND_MAX * 2.0 - 1.0;    // losowa liczba w zakresie od -1 do 1 (współrzędna y)
        norm = hypot(x, y);                         // obliczenie odleglości punktu od początku układów współrzędnych
        
        if(norm <= 1) circleCount++;                // jeśli punkt znajduje się wewnątrz koła, zwiększamy licznik punktów koła
        //printf("%ld) x = %f, y = %f, norm = %f\n", tid, x, y, norm);
    }

    printf("Wątek %ld zakończył działanie\n", tid);
    pthread_exit((void*)circleCount);
}

int main(int argc, char *argv[]){
    srand(time(NULL));

    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;
    long tid, circCount;
    int retval;
    void *circ;
    double ratios, pi;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for(tid = 0; tid < NUM_THREADS; tid++){
        printf("Main: Tworzenie wątku %ld\n", tid);

        retval = pthread_create(&threads[tid], &attr, approxPi, (void *)tid);
        if(retval){
            printf("Wystąpił bład przy tworzeniu wątku. Funkcja pthread_create() zwróciła %d\n", retval);
            exit(-1);
        }
    }

    pthread_attr_destroy(&attr);

    for(tid = 0; tid < NUM_THREADS; tid++) {
        retval = pthread_join(threads[tid], &circ);

        ratios += (double)(long)circ/SAMPLE_SIZE;

        if(retval){
            printf("Wystąpił błąd. Funkcja pthread_join() zwróciła %d\n", retval);
            exit(-1);
        }
        printf("Main: Zakończono łączenie z wątkiem %ld.\n" , tid);
    }

    pi = ratios/NUM_THREADS * 4.0;

    printf("Main: Program zakończył działanie. Wyliczona liczba π = %f\n", pi);

    pthread_exit(NULL);
}