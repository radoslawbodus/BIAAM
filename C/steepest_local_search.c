#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "random_permutation.h"
#include "read_and_allocate_data.h"
#include "create_distance_matrix.h"
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

void steepest_local_search(double **distance_matrix, int *solution, int size, long *iterations_done, long *evaluations_done)
{
	long start_mili, end_mili;
	struct timeval timecheck;
	
	int counter = 0;
	int count_evaluations = 0;
	double current_fitness = fitness(solution, distance_matrix, size);
	double previous_fitness = current_fitness;
	double best_fitness = current_fitness;
	int i, j;
	int type = 0;	
	
	
	int stop_cond = 0;
	while (1)
	{
		counter++;	
		
		double best_delta = 0;
		int best_i, best_j;
		double current_delta = 0;

		for (int i = 0; i < size - 1; i++)
		{
			for (int j = i + 1; j < size; j++)
			{
				if ((current_delta = delta_two_nodes_exchange(solution, i, j, distance_matrix, size)) < best_delta)

				{
					best_delta = current_delta;
					best_i = i;
					best_j = j;
					type = 0;
				}
				if ((current_delta = delta_two_edge_exchange(solution, distance_matrix, size, i, j)) < best_delta)

                                {
                                        best_delta = current_delta;
                                        best_i = i;
                                        best_j = j;
                                        type = 1;
                                }
				count_evaluations += 2;

			}
		}
		if (best_delta < 0)
                {
                        if (type == 0)
                                swap(&solution[best_i], &solution[best_j]);
                        else if (type == 1)
                                reverse_route(&solution[best_i+1], &solution[best_j]);
                }

		else
		{
			break;
		}

	}
	*iterations_done = counter; // count_evaluations
	*evaluations_done = count_evaluations;
	
	double best_solution_found = fitness(solution, distance_matrix, size);


	return;
}

