#include <stdio.h>
#include <stdlib.h>

int main(void) {
    pid_t child_pid;
    int status;
    
    child_pid = fork();
    if (child_pid  == 0) {
      printf("HI! I'm a new process!\nAnd i have my ");
      printf("child_pid = %d\n", getpid());
      printf("and i know my parent pid = %d\n", getppid());
      exit(1);
    } else {
      wait(&status);
      printf("status = %d\n", WEXITSTATUS(status));
      
      printf("\nI'm his Pa, and I have my ");
      printf("Parent id = %d\n", getpid());
      printf("and his Grandpa id = %d\n", getppid());
    }
    
    return 0;
}
