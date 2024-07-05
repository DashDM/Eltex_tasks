/*
2]
научиться блокировать sigint, чтобы не удалось закрыть программу с помощью ctrl+c. sigprocmask() 
*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
   sigset_t mask;
   
   // создаём маску сигналов
   sigemptyset(&mask);
   sigaddset(&mask, SIGINT); // добавляем SIGINT в маску
   
   // блокировка сигнала SIGINT
   if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
      perror("sigprocmask");
      exit(EXIT_FAILURE);
   }
   
   printf("'Ctrl + C' не спасёт!\n");
   
   while (1) {
      printf("ОЙ-ЁЙ-ЁЙ\n");
      sleep(5);
      printf("АЙ-ЯЙ-ЯЙ\n");
      sleep(5);
      printf("copy to another terminal:\n");
      printf("1) ps aux [find <pid> of this process -> ./имя_бинарника]\n");
      printf("2) kill -15 <pid> [15 -> SIGTERM]\n");
      sleep(5);
   }
   
   return 0;
}
