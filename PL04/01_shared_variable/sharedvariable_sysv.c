//compile with: gcc -Wall sharedvariable_sysv.c semlib.c -o svar
//using SysV semaphores

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "semlib.h"
#include <sys/wait.h>

/*insert here the other libraries needed*/


int * shared_var;
int shmid, semid;

void worker(){
	usleep(1000000 + rand()%11*100000); //uses microseconds sleep and waits 1 to 2 seconds in periods of 0.1 secs

	 sem_wait(semid, 0);
	(*shared_var)++;
	 sem_post(semid, 0);

}

int main(int argc, char *argv[])
	{
	int i, n_procs;

	//seed the rand library
	srand((unsigned int)time(NULL));

	if(argc!=2) {
		printf("Wrong number of parameters\n");
		exit(0);
	}

	n_procs=atoi(argv[1]);
	printf("We will create %d process!\n",n_procs);


	// Create shared memory
	if ((shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0766)) < 0) {
		perror("Error in shmget with IPC_CREAT\n");
		exit(1);
	}

	// Attach shared memory
	if ((shared_var = (int *) shmat(shmid, NULL, 0)) == (int*)-1) {
		perror("Shmat error!");
		exit(1);
	}
	
	// Create semaphores
	if ((semid = sem_get(1, 1)) == -1) {
		perror("Failed to initialize semaphore");
		exit(1);
	}


	// initialize variable in shared memory
	*shared_var = 1000;

	// Create worker processes
	for(int i=0; i < n_procs ; ++i){
		pid_t id = fork();
		if (id == 0){
			printf("Worker of pid_t %d spawned!\n", getpid());
			worker();
			exit(EXIT_SUCCESS);
		}
	}
	
	// Wait for all worker processes
	for(int i=0; i < n_procs ; ++i)
		wait(NULL);
	
	// print final result
	printf("The value writen on shared memory is %d\n", *shared_var);

	// remove resources (shared memory and semaphores)
	sem_close(semid);
	shmdt(shared_var);
	shmctl(shmid,IPC_RMID,NULL);

	exit(0);
}
