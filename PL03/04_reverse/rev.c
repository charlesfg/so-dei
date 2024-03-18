#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define PROC 5

int main(void) {
  pid_t childpid;
  int n;

  for (n=0;n<PROC-1;n++){
	childpid = fork();

	if (childpid != 0){
		wait(NULL);
		if (n!=0) printf("I am %d and my father is %d\n", getpid(),getppid());
		else printf("I am %d, the original\n", getpid());
		exit(0);
		}
  } // do for
  printf("I am %d and my father is %d\n", getpid(),getppid());
  return 0;
}
