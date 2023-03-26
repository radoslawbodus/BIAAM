// random_permutation.c - Create a random permutation of size n
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "random_permutation.h"

/*
int main(void)
{
	random_permutation.c - Create a random permutation of size nint permutation_size;
	int *arr;
	
	permutation_size = 10;

	arr = random_permutation(permutation_size);
	print_array(arr, permutation_size);


	return 0;
}
*/

int *random_permutation(int size)
{
	long time_current_micro_s;
	struct timeval timecheck;
	int j;

	int *arr = (int *)malloc(sizeof(int) * size);

	for (int i = 0; i < size; i++)
		arr[i] = i;
	
	for (int i = size - 1; i >= 1; i--)
	{
		gettimeofday(&timecheck, NULL);
		time_current_micro_s = timecheck.tv_usec;
		
		srand((unsigned int) time_current_micro_s);
		j = rand() % (i + 1);
	
		swap(&arr[i], &arr[j]);	
	}

	return arr;
}

void random_permutation_time_test(int size)
{
	int *arr;
	arr = random_permutation(size);
	free(arr);
}

void print_array(int *arr, int size)
{
	for (int i = 0; i < size; i++)
		printf("%d ", arr[i]);

	printf("\n");

	return;
}

void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;

	return;
}
