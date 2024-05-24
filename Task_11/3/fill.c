#include "life_simulation.h"

// заполняем магазины случайным числом товаров в пределах (от 8тыс. до 13тыс.)
void fill_shops(void) {
    for (int i = 0; i < NUM_SHOPS; i++) {
        shops[i] = rand() % 5001 + 8000;
    }
}
// заполняем первоначальные потребности в пределах (от 100к до 120к)покупателя и назначаем ему идентификатор
void fill_needs(struct Customer *c_person, int i) {
    c_person->id = i;
    c_person->needs = rand() % 20001 + 100000;
}
