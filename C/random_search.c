#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "random_permutation.h"
#include "read_and_allocate_data.h"
#include "create_distance_matrix.h"
#include "random_search.h"
#include "utils.h"


/*
int main(void)
{
	char *file_path = "gr137.tsp";
	
	int *solution;
	int size;
	srand((unsigned int) time(NULL));

        double **coordinates_cities_array = coordinates_cities(file_path, &size);
        double **distance_matrix_cities = distance_matrix(coordinates_cities_array, 137);
	
        //print_matrix(distance_matrix_cities, 137);
	
	solution = random_permutation(137);
	random_search(distance_matrix_cities, solution, 137);

	
	return 0;
}
*/
void random_search(double **distance_matrix, int *solution, int size, int time_mili, long *iterations_done)
{
	long start_mili, end_mili;
	struct timeval timecheck;
	int *random_solution;
	int counter = 0;
	random_solution = random_permutation(size);

	gettimeofday(&timecheck, NULL);
        start_mili = (long) timecheck.tv_sec * 1000 + (long) timecheck.tv_usec / 1000;

	while (1)
	{
		gettimeofday(&timecheck, NULL);
	        end_mili = (long) timecheck.tv_sec * 1000 + (long) timecheck.tv_usec / 1000;
		if (end_mili -start_mili > time_mili)
			break;
		

		shuffle(random_solution, size);
		if (fitness(random_solution, distance_matrix, size) < fitness(solution, distance_matrix, size))
		{
			copy_solution(solution, random_solution, size);
		}
		counter++;

	}
	*iterations_done = counter;
	free(random_solution);
	return;
}


