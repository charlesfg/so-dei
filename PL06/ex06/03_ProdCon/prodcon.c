#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


#define WAIT_LIST 10
#define CONSUMERS 5
#define PRODUCERS 10
#define SIZE 256
#define MAX_PRODUCED_ITEMS 100

typedef struct c {
	char message [SIZE];
	int entry_number;
} slot;

slot table [WAIT_LIST];
int writepos = 0;
int readpos = 0;
int produced_items=0;
int consumed_items=0;

pthread_mutex_t lock_write = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_read = PTHREAD_MUTEX_INITIALIZER;

// #1

// Complete the code

// End of #1

void * producer (void * id_ptr);
void * consumer (void *);
void put_data_at (int pos, slot * regist);
void get_data_at (int pos, slot * regist);

int main (){

	int id_p[PRODUCERS];
	int id_c[CONSUMERS];
	pthread_t prods[PRODUCERS];
	pthread_t cons[CONSUMERS];
	int i;
	srand(pthread_self());
	for (i=0; i<PRODUCERS; i++) {
		id_p[i]=i;
		if (pthread_create (&prods[i], NULL, producer, &id_p[i])!=0)
			perror("Error creating producer!");
		
	}
	for (i=0; i<CONSUMERS; i++) {
		id_c[i]=i;
		if (pthread_create (&cons[i], NULL, consumer, &id_c[i])!=0)
			perror("Error creating consumer!");
	}

	for (i=0; i<PRODUCERS; i++)
		pthread_join (prods[i], NULL);
	for (i=0; i<CONSUMERS; i++)
		pthread_join (cons[i], NULL);

// #2

// Complete the code

// End of #2
	printf("\nProgram statistics:\nproduced_items=%d , consumed_items=%d\nwritepos=%d , readpos=%d\n", produced_items, consumed_items, writepos,readpos);
	pthread_exit (NULL);

}

void * producer (void * id_ptr){

	int id = *((int *)id_ptr);
	slot * regist = (slot *)malloc(sizeof(slot));

	int number = 0;
	while (1){
 		sleep(rand()%10);
		number = rand()%1000000000;
		regist->entry_number = number; // production registry number (randomly chosen)
		sprintf(regist->message, "service produced by %d", id);

// #3

// Complete the code

// End of #3

		pthread_mutex_lock (&lock_write);
		put_data_at(writepos, regist);
		writepos = (writepos+1)%WAIT_LIST;
		pthread_mutex_unlock (&lock_write);

// #4

// Complete the code

// End of #4
	} 
	pthread_exit (NULL);
}

void * consumer (void * id_ptr){
	int id = *((int *)id_ptr);
	int readpos = 0;
	slot regist;
	while (1){

// #5

// Complete the code

// End of #5

		pthread_mutex_lock (&lock_read);
		get_data_at(readpos,&regist);
		readpos=(readpos+1)%WAIT_LIST;
		pthread_mutex_unlock (&lock_read);

// #6

// Complete the code

// End of #6	

		printf("Consumer %d: %d - %s\n", id,regist.entry_number, regist.message);

	}
	pthread_exit (NULL);
}

void put_data_at (int pos, slot * regist) {
	memcpy(&(table[pos]), regist, sizeof(slot));
}

void get_data_at (int pos, slot * regist) {
	memcpy(regist, &(table[pos]), sizeof(slot));
}
