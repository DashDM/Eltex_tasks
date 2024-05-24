#ifndef __LIFE_SIMULATION_H__
#define __LIFE_SIMULATION_H__
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_SHOPS 5
#define NUM_CUSTOMERS 3

extern pthread_mutex_t m[];

// объявляем массив магазинов (каждая ячейка - магазин)
extern int shops[];
// структура для потока-покупателя (содержит идентификатор и необходимое число товаров)
struct Customer {
    int id;
    long needs;
};
// структура для потока-погрузчика (содержит идентификатор)
struct Loader {
    int id;
    int loader_tank;
};

void fill_shops(void);
void fill_needs(struct Customer *person, int i);
void *thread_customer(void *arg);
void *thread_loader(void *arg);
#endif
