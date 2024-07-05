/*
1]
с помощью sigaction просто переопределить siguser1. просто когда бы прога его получала, то мы бы выводили на экран "получено siguser1".
в main переопределяем диспозицию и уходим в беск. цикл. в другом терминале узнаём pid через ps aux. через утилиту kill -SIGUSR1 <pid> или другую прогу посылаем
siguser1 процессу и проверяем результат.
*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handler(int signum)
{
   printf("Received SIGUSR1\n");
   exit(EXIT_SUCCESS); // можно убрать, тогда можно беск. kill'ять и получать сообщение о полученном сигнале
}

int main(void)
{
   struct sigaction act = { 0 };
   act.sa_handler = &handler;
   
   // регистрируем функцию - обработчик сигнала
   if (sigaction(SIGUSR1, &act, NULL) == -1) {
      perror("sigaction");
      exit(EXIT_FAILURE);
   }
   
   while (1) {
      sleep(2);
   }
   
   return 0;
}
