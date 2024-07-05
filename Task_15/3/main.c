/*
3]
написать eventloop с поддержкой sigwait(). т.е. прога в беск цикле while вызывает sigwait siguser1, т.е. ожидает
и когда выходит из sigwait пишет пришёл siguser1. открываем ещё 2 терминала. во 2-м посылаем siguser1 постоянно в эту программу
а в 3 запускаем диспетчер задач и смотрим как этот процесс потребляет процессорное время.
*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handler(int sig) {
    printf("Received signal %d\n", sig);
}

int main(void)
{
   sigset_t mask;
   
   // создаём маску сигналов
   sigemptyset(&mask);
   sigaddset(&mask, SIGUSR1); // добавляем сигнал SIGUSR1 в маску
   
   signal(SIGUSR1, handler); // устанавливаем обработчик сигнала
   
   while (1) {
      int sig = 0;
      sigwait(&mask, &sig); // ожидаем сигнал SIGUSR1
      printf("Received: %d | SIGUSR1\n", sig);
   }
   
   return 0;
}
