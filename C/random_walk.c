#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "random_permutation.h"
#include "read_and_allocate_data.h"
#include "create_distance_matrix.h"
#include "random_walk.h"
#include "utils.h"


/*
int main(void)
{
	char *file_path = "gr137.tsp";
	
	int *solution;
	int size;

        double **coordinates_cities_array = coordinates_cities(file_path, &size);
        
	double **distance_matrix_cities = distance_matrix(coordinates_cities_array, 137);

        //print_matrix(distance_matrix_cities, 532);
		
	solution = random_permutation(137);
	
	random_walk(distance_matrix_cities, solution, 137);
		
	
	return 0;
}
*/

void random_walk(double **distance_matrix, int *solution, int size, int time_micro, long *iterations_done)
{
	long start_micro, end_micro;
	struct timeval timecheck;
	int *random_solution = random_permutation(size);
	copy_solution(random_solution, solution, size);
	
	int counter = 0;
	double current_fitness = fitness(random_solution, distance_matrix, size);
	double current_delta = 0;
	double best_fitness = current_fitness;
	int i, j;
	
	
	gettimeofday(&timecheck, NULL);
        
	start_micro = (long) timecheck.tv_sec * 1000000 + (long) timecheck.tv_usec;
	printf("%d\n", time_micro);	
	int stop_cond = 0;
		
	while (1)
	{
		gettimeofday(&timecheck, NULL);
	        end_micro = (long) timecheck.tv_sec * 1000000 + (long) timecheck.tv_usec;
		if (end_micro - start_micro > time_micro)
			break;
		
		
		i = rand() % size;
		j = rand() % (size - 1);

		if (i == j)
			j = size - 1;
		
		if ((current_fitness += delta_two_nodes_exchange(random_solution, i, j, distance_matrix, size)) < best_fitness)
		{
			swap(&random_solution[i], &random_solution[j]);
			copy_solution(solution, random_solution, size);
			best_fitness = fitness(solution, distance_matrix, size);
			
		}
		else
		{
			swap(&random_solution[i], &random_solution[j]);
		}

		counter++;

	}
	*iterations_done = counter;
	
	free(random_solution);	
	return;
}


