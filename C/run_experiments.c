#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "create_distance_matrix.h"
#include "random_permutation.h"
#include "read_and_allocate_data.h"
#include "steepest_local_search.h"
#include "greedy_local_search.h"
#include "random_walk.h"
#include "random_search.h"
#include "utils.h"

int main(void)
{
	char *file_path = "gr137.tsp";
	int i, size;
	int iterations = 100, flag = 0;
	int time_mili;
	double **coordinates_cities_array = coordinates_cities(file_path, &size);
        double **distance_matrix_cities = distance_matrix(coordinates_cities_array, size);	
	int *solution = random_permutation(size);
	
	long start_mili, end_mili;
        struct timeval timecheck;
	
	srand(time(NULL));
	
	for (i = 0; i < iterations; i++)
	{
		printf("i: %d\n", i);

		flag = 0;
		shuffle(solution, size);
		
		gettimeofday(&timecheck, NULL);
	        start_mili = (long) timecheck.tv_sec * 1000 + (long) timecheck.tv_usec / 1000;
		greedy_local_search(distance_matrix_cities, solution, size);
		gettimeofday(&timecheck, NULL);
                end_mili = (long) timecheck.tv_sec * 1000 + (long) timecheck.tv_usec / 1000;
		time_mili = end_mili - start_mili;
	
		save_as_csv(solution, fitness(solution, distance_matrix_cities, size), size, "gr137_greedy.csv", flag);

		shuffle(solution, size);

		steepest_local_search(distance_matrix_cities, solution, size);
		save_as_csv(solution, fitness(solution, distance_matrix_cities, size), size, "gr137_steepest.csv", flag);

		shuffle(solution, size);

		random_walk(distance_matrix_cities, solution, size, time_mili);
		save_as_csv(solution, fitness(solution, distance_matrix_cities, size), size, "gr137_random_walk.csv", flag);

		shuffle(solution, size);
		
		random_search(distance_matrix_cities, solution, size, time_mili);
		save_as_csv(solution, fitness(solution, distance_matrix_cities, size), size, "gr137_random_search.csv", flag);

	}

	return 0;
}
