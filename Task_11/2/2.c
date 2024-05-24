#include <stdio.h>
#include <pthread.h>

#define thread_num 18

long sum = 0;
pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;

void *thread_calc(void *args) {
    int i, tmp;
    
    for (i = 0; i < 10000000; i++) {
        pthread_mutex_lock(&m1);
        sum++;
        pthread_mutex_unlock(&m1);
    }
}

int main(void) {
    int *s;
    pthread_t thread[thread_num];
    
    for (int i = 0; i < thread_num; i++) {
        pthread_create(&thread[i], NULL, thread_calc, NULL);
    }
    
    for (int j = 0; j < thread_num; j++) {
        pthread_join(thread[j], (void **)&s);
    }
    
    printf("sum = %ld\n", sum);
    
    return 0;
}
