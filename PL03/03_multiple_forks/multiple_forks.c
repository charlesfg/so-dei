// Creates 7 additional processes
// Prints a result such as (one by line): 01221201222122

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {

    pid_t c_pid;
  for (int i=0;i<3;i++){
	c_pid = fork();
    if (c_pid == 0){
        printf("A New Process was spawned!\n");
        printf("[child] %d: %d\n",getpid(),i);
        exit(0);
    }
	printf("%d: %d\n",getpid(),i);
    waitpid(c_pid,NULL,0);
	}
}

/*
a1
    b1
    b2
        e2
        e3
    b3
        f3
a2
    c2
    c3
        g3
        h3
a3 
    d3

57067: 0
57067: 1
57067: 2
57068: 0
57068: 1
57068: 2
57069: 1
57069: 2
57070: 1
57070: 2
57071: 2
57072: 2
57073: 2
57074: 2


P0:
i=0 -> fork P1 i=0
               i=1 -> fork P3 i=1
                              i=2 -> fork P5 i=2
               i=2 -> fork P4 i=2
        
i=1 -> fork P2 i=1
               i=2 -> fork P6 i=2
i=2 -> fork P3 i=2

*/