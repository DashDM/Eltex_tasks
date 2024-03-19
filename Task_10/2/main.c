#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void) {
  int status1, status2;
  pid_t process1;
  
  process1 = fork();
  if (process1 == 0) {
      printf("process1:\n");
      printf("  ppid = %d\n", getppid());
      printf("  pid = %d\n", getpid());
      
      int status3, status4;
      pid_t process3 = fork();
      if (process3  == 0) {
          printf("process3:\n");
          printf("  ppid = %d\n", getppid());
          printf("  pid = %d\n", getpid());
          exit(3);
      } else {
          wait(&status3);
          printf("!status3 = %d\n\n", WEXITSTATUS(status3));
          
          pid_t process4 = fork();
          if (process4 == 0) {
              printf("process4:\n");
              printf("  ppid = %d\n", getppid());
              printf("  pid = %d\n", getpid());
              exit(4);
          } else {
              wait(&status4);
              printf("!status4 = %d\n\n", WEXITSTATUS(status4));
          }
    }
    
    exit(1);
  } else {
      wait(&status1);
      printf("!status1 = %d\n\n", WEXITSTATUS(status1));
      
      pid_t process2 = fork();
      if (process2 == 0) {
          printf("process2:\n");
          printf("  ppid = %d\n", getppid());
          printf("  pid = %d\n", getpid());
          
          int status5;
          pid_t process5 = fork();
          if (process5 == 0) {
              printf("process5:\n");
              printf("  ppid = %d\n", getppid());
              printf("  pid = %d\n", getpid());
              exit(5);
          }
          wait(&status5);
          printf("!status5 = %d\n\n", WEXITSTATUS(status5));
          exit(2);
        } else {
            wait(&status2);
            printf("!status2 = %d\n\n", WEXITSTATUS(status2));
            printf("The End\n");
      }
  }
  
  return 0;
}
