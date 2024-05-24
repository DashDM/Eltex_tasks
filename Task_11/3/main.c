#include "life_simulation.h"

pthread_mutex_t m[NUM_SHOPS] = {PTHREAD_MUTEX_INITIALIZER};
int shops[NUM_SHOPS];

int main(void) {
    srand(time(NULL));
    // объявляем массив структур-покупателей и инициализируем структуру-погрузчика
    struct Customer c_person[NUM_CUSTOMERS];
    struct Loader l_person = {NUM_CUSTOMERS, 5000};
    // объявляем массив потоков-покупателей и поток-погрузчика
    pthread_t th_customers[NUM_CUSTOMERS];
    pthread_t th_loader;
    // заполняем магазины и формуриуем данные покупателей
    fill_shops();
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        fill_needs(&c_person[i], i);
    }
    // создание потоков-покупателей
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_create(&th_customers[i], NULL, thread_customer, (void *)&c_person[i]);
    }
    // создаём поток-погрузчик
    pthread_create(&th_loader, NULL, thread_loader, (void *)&l_person);
    // ждём завершения потоков-покупателей
    for (int i = 0; i < NUM_SHOPS; i++) {
        pthread_join(th_customers[i], NULL);
    }
    
    // главный поток завершает поток-загрузчик
    pthread_cancel(th_loader);
    
    return 0;
}

