// Compile as: gcc matrix.c -lpthread -D_REENTRANT -Wall -o matrix

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define M 3 //first matrix lines
#define K 2 //first matrix columns; second matrix lines
#define N 3 //second matrix columns

struct v {
  int row;
  int column;
};

typedef struct v coordenate;
void *worker(void* coord);
void *show_matrix();	// prints matrices on screen

pthread_t my_thread[(M*N)+1];
int A[M][K] = { {1,4}, {2,5}, {3,6} };
int B[K][N] = { {8,7,6}, {5,4,3} };
int C[M][N];

// creates mutex




void print_matrix(){ 
// prints matrices on screen when having exclusive access
    
}

int main(void) {
  // creates threads and sends each the coordinates of the element to calculate
  
  // waits for threads to finish
  
  // prints final results of the calculation and leaves
  
}


void *worker(void* coord) {
  // calculates each matrix element in the coordinate received
  
}

void *show_matrix() {
// prints the status of the matrix in each second, along a period of 10 seconds
  
}
