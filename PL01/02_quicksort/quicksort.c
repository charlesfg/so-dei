#include <stdio.h>
#include <stdlib.h>

/* Swaps values	o and d */
void swap(int *o, int *d){
  int t;
  t = *o;					/* Inserts in t the value pointed by o */
  *o = *d;				/* o points to the same value as d */
  *d = t;					/* d points to t */
}

/* Divides array and returns the index of the partition */
int partition(int s[], int l, int h){
  int i;
  int firsthigh;
  firsthigh = l;
  for (i = l; i < h; i++)		       /* Go through the array */
    if (s[i] < s[h]){			         /* If the value is lower than the last position */
      swap(&s[i], &s[firsthigh]);	 /*  swaps the values of the positions in the array */
      firsthigh++;					       /*  and increases the index of the first element */
    }
  swap(&s[h], &s[firsthigh]);		   /* Inserts the value of the next key */
  return firsthigh;
}

/* s: array to order */
/* l: lower index of the array */
/* h: higher index of the array */
int quicksort(int s[], int l, int h){
  int p;
  if ((h-l) > 0){					        /* If the number of member is higher than 1 */
    p = partition(s, l, h);			  /*  finds the index for the partition */
    quicksort(s, l, p-1);			    /*  and sorts the lower values */
    quicksort(s, p+1, h);			    /*  and sorts the higher values */
  }
  return 0;
}

/* Prints the array s (n elements) in the screen	*/
void print_array(int s[], int n){
  int i;
  for (i = 0; i < n-1; i++){
    printf("%d, ", s[i]);
  }
  printf("%d\n", s[n-1]);
}

int main(void){
  int x[] = {1, 4, 2, 3, 7, 6};
  
  /* Print original array */
  printf("Original array:\n");    /* Shows initial array */
  print_array(x,sizeof(x)/sizeof(int));
  
  /* Sort array */
  quicksort(x,0, sizeof(x)-1);    /* Orders array using quicksort() */
  
  /* Print sorted array */
  printf("Ordered array:\n");
  print_array(x,sizeof(x)/sizeof(int)-1);
  
  return 0;
}