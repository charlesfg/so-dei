// Creates 7 additional processes
// Prints a result such as (one by line): 01221201222122

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {

    int n=4;
    pid_t c_pid;

    for (int i=0;i<n;i++){
        c_pid = fork();
        if (c_pid != 0){
            wait(NULL);
            if(i==0){
                printf("I'm the %d ORIGINAL process and my parent is %d!\n", getpid(), getppid());               
            } else {
                   printf("I'm the %d process and my parent is %d!\n", getpid(), getppid()); 
            }
                    
            exit(0);
        }
          
	}
    
}

