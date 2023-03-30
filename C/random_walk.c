// random_walk.c - Random walk algorithm for tsp
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

        double **coordinates_cities_array = coordinates_cities(file_path);
        
	double **distance_matrix_cities = distance_matrix(coordinates_cities_array, 137);
	
        //print_matrix(distance_matrix_cities, 532);
	srand(time(NULL));	
	solution = random_permutation(137);
	
	clock_t start = clock();
	steepest_local_search(distance_matrix_cities, solution, 137);
	clock_t end = clock();

	printf("It took %lf seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
	
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
	int count_evaluations = 0;
	double current_fitness = fitness(solution, distance_matrix, size);
	double best_fitness = current_fitness;
	int i, j;
	int type = 0;	
	int best_i, best_j;
	
	int stop_cond = 0;

	gettimeofday(&timecheck, NULL);
	start_micro = (long) timecheck.tv_sec * 1000000 + (long) timecheck.tv_usec;

	while (1)
	{

		gettimeofday(&timecheck, NULL);
                end_micro = (long) timecheck.tv_sec * 1000000 + (long) timecheck.tv_usec;
                if (end_micro - start_micro > time_micro)
                        break;

		counter++;	
		
		i = rand() % size;
                j = rand() % (size - 1);

                if (i == j)
                        j = size - 1;


		if ((current_fitness += delta_two_nodes_exchange(random_solution, i, j, distance_matrix, size)) < best_fitness)

		{
			swap(&random_solution[i], &random_solution[j]);
			copy_solution(solution, random_solution, size);
			best_fitness = current_fitness;
		}
		else
		{	
	                swap(&random_solution[i], &random_solution[j]);
		}

	}
	*iterations_done = counter; 
	
	return;
}

