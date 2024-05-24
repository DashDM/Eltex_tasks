#include "life_simulation.h"

// поток-покупатель
void *thread_customer(void *arg) {
    // инициализируем структуру покупателя
    struct Customer *c_person = (struct Customer *)arg;
    // while: пока нужды покупателя не станут равны 0
    while (c_person->needs > 0) {
        // for: поток ищет свободный магазин
        for (int i = 0; i < NUM_SHOPS; i++) {
            // найденный свободный магазин блокируется этим потоком
            pthread_mutex_lock(&m[i]);
            // если продукты в магазине есть
            if (shops[i] > 0) {
                // объявляем переменную, в которой будем хранить число взятых из магазина продуктов
                int products_taken;
                // если требуется закупить больше, чем есть в магазине
                if (c_person -> needs >= shops[i])
                    // то покупатель забирает всё
                    products_taken = shops[i];
                else
                    // иначе, покупатель берёт ровно столько, сколько ему нужно
                    products_taken = c_person -> needs;
                // покупатель уменьшает кол-во необходимого товара на число товаров, которое он взял в магазине
                c_person -> needs -= products_taken;
                // вывод инфы
                printf("Customer %d | entered shop %d | take %d products | remaining needs: %ld\n", c_person->id, i+1, products_taken, c_person->needs);
                // обновляем 'ассортимент' магазина
                shops[i] -= products_taken;
            }
            // поток уходит из 'критической зоны', предоставляя другим потокам доступ к магазину
            pthread_mutex_unlock(&m[i]);
            // и в сон на 2 сек
            sleep(2);
        }
    }
    return NULL;
}
// поток-погрузчик
void *thread_loader(void *arg) {
    // по аналогии инициализируем структуру погрузчика
    struct Loader *l_person = (struct Loader *)arg;
    // бесконечный цикл, т.к. по условию, погрузчика может остановить только главный поток (main крч.)
    while (1) {
        // ищем свободный магазин
        for (int i = 0; i < NUM_SHOPS; i++) {
            // поток-погрузчик блокирует его
            pthread_mutex_lock(&m[i]);
            // далее погрузчик пополняет запасы магазина
            shops[i] += l_person->loader_tank;
            // выводи всю инфу о себе красивом
            printf("Loader %d | entered shop %d | place %d products | total products: %d\n", l_person->id, i+1, l_person->loader_tank, shops[i]);
            // освобождает магазин
            pthread_mutex_unlock(&m[i]);
            // и в сон на 1 сек
            sleep(1);
        }
    }
    
    return NULL;
}
