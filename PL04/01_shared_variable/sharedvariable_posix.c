//compile with: gcc -Wall -pthread sharedvariable_posix.c -o svar
//using POSIX named semaphores

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h> // include POSIX semaphores
#include <fcntl.h>     // For O_* constants
#include <sys/stat.h>


/*insert here the other libraries needed*/



int * shared_var;
int shmid;
sem_t *mutex;

void worker()
	{
	usleep(1000000 + rand()%11*100000); //uses microseconds sleep and waits 1 to 2 seconds in periods of 0.1 secs
	
	// ensure mutual exclusion
	sem_wait(mutex);
	(*shared_var)++;
	sem_post(mutex);
	
	}

int main(int argc, char *argv[])
	{
	int i, n_procs;

	if(argc!=2) {
		printf("Wrong number of parameters\n");
		exit(0);
		}

	n_procs=atoi(argv[1]);

	// Create shared memory
	if ((shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0766)) < 0) {
		perror("Error in shmget with IPC_CREAT\n");
		exit(1);
	}

	// Attach shared memory
	if ((shared_var = (int *) shmat(shmid, NULL, 0)) == (int*)-1) {
		perror("Shmat error!");
		printf("Errno == %d\n", errno);
		exit(1);
	}
	
	// Create semaphore
	sem_unlink("MUTEX");
    mutex=sem_open("MUTEX",O_CREAT|O_EXCL,0700,1);

	// initialize variable in shared memory
	*shared_var = 1000;

	// Create workers
	for(i=0;i<n_procs;i++){
		if(fork()==0){
			printf("Worker %d created and ready for business!\n",getpid());
			worker();
			printf("Worker %d leaving!\n",getpid());
			exit(0);
			}
		}

	// Wait for workers
	for(i=0;i<n_procs;i++) wait(NULL); // parent waits for all children

	// print final result
	printf("Final result= %d\n",*shared_var);

	// remove resources
	sem_close(mutex);
	sem_unlink("MUTEX");
	shmdt(shared_var);
	shmctl(shmid,IPC_RMID,NULL);

	exit(0);
}
