/******************************************************************************
* FILE: shop.c
* DESCRIPTION:
*   A shop has several employees and one foreman, each represented by a thread.
*   The threads are created by the main thread.
*   - Some threads represent the shop employees that sell items for 10 eur
*     and put the money in a common safe.
*   - One thread represents the foreman that watches the value of the money. When it
*     reaches a predefined limit (CASH_LIMIT), he removes an amount of CASH_LIMIT
*     money from the safe, for security reasons.
*   - The program ends after NSALES
******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS  8	// Total number of threads (>1)
#define NSALES 30	// Max number of sales made by all employees
#define CASH_LIMIT 50	// Limit of the 'count' variable before it is reset

int     	sales = 0;
int		money = 0;

/* initialize money_mutex and money_amount_cv condition variable */
pthread_mutex_t money_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t 	money_amount_cv = PTHREAD_COND_INITIALIZER;

void *employee(void *t)
{
  int my_id = *((int *)t);

  printf("Starting employee(): thread %d\n", my_id);

  while (1) {
	pthread_mutex_lock(&money_mutex);
	if (sales < NSALES) {
		money+=10;	// increment amount of money in the safe
		sales++;	// increment number of sales
		pthread_cond_signal(&money_amount_cv);
		printf("Employee: sell number %d! Just sent signal. safe=%d\n",sales,money);
	}
	else
	{
		pthread_mutex_unlock(&money_mutex);
		break;
	}
	pthread_mutex_unlock(&money_mutex);
	sleep(1); // to force alternation between mutex locks
	}

  pthread_exit(NULL);
}

void *foreman(void *t)
{
  int my_id = *((int *)t);

  printf("Starting foreman(): thread %d\n", my_id);

  while (1) {
	pthread_mutex_lock(&money_mutex);

	while (money < CASH_LIMIT && sales < NSALES){
		pthread_cond_wait(&money_amount_cv, &money_mutex);
 	}
	if (money > CASH_LIMIT) {
		money -= CASH_LIMIT;
		printf("Foreman removed money! New amount = %d\n", money);
		}
	if (sales >= NSALES) {
		pthread_mutex_unlock(&money_mutex);
		break;
		}

	pthread_mutex_unlock(&money_mutex);
	}

  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  int i;
  int id[NUM_THREADS];
  pthread_t threads[NUM_THREADS];

  /* Create threads */
  id[0]=0;
  pthread_create(&threads[0], NULL, foreman, &id[0]);
  for(i = 1; i < NUM_THREADS; i++) {
	id[i]=i;
  pthread_create(&threads[i], NULL, employee, &id[i]);
	}

  /* Wait for all threads to complete */
  for (i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("Main(): Waited and joined with %d threads. Final money in safe = %d. Done!\n",
          NUM_THREADS, money);

  /* Clean up and exit */
  pthread_mutex_destroy(&money_mutex);
  pthread_cond_destroy(&money_amount_cv);
  //pthread_exit (NULL);

}
