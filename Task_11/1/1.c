#include <stdio.h>
#include <pthread.h>

#define thread_num 5

void *thread_calc(void *args) {
    int *i = (int *)args;
    printf("*thread num %d\n", *i);
}

int main(void) {
    int *s;
    int i_memory[thread_num];
    pthread_t thread[thread_num];
    
    for (int i = 0; i < thread_num; i++) {
        i_memory[i] = i;
        pthread_create(&thread[i], NULL, thread_calc, (void *)&i_memory[i]);
    }
    
    for (int j = 0; j < thread_num; j++) {
        pthread_join(thread[j], (void **)&s);
    }
    
    return 0;
}
