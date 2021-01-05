#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *printHello(void *_tid){
    printf("Hello SCR. Written by thread %ld\n", (long)_tid);   // wypisujemy napis

    pthread_exit(NULL);     // zamykamy wątek
}

int main(){
    pthread_t thread1, thread2, thread3;
    long threadN;
    int t;

    for(threadN = 0; threadN < 3; threadN++){   

        if(t = pthread_create(&thread1, NULL, printHello, (void *) threadN)){   // tworzymy wątek i jeśli wystąpił błąd podczas procesu tworzenia
            printf("Błąd przy tworzeniu wątku: %d\n", t);       // wyświetlamy komunikat z numerem błędu
            exit(-1);       // i zamykamy program
        }

    }

    pthread_exit(NULL);     // ostatnia czynność głównego programu

    return 0;
}
