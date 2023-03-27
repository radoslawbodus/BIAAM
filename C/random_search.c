#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "random_permutation.h"
#include "read_and_allocate_data.h"
#include "create_distance_matrix.h"

double fitness(int *solution, double **distance_matrix, int size);
void random_search(double **distance_matrix, int *solution, int size);
void copy_solution(int *target, int *source, int size);


int main(void)
{
	char *file_path = "att532.tsp";
	
	int *solution;

        double **coordinates_cities_array = coordinates_cities(file_path);
        double **distance_matrix_cities = distance_matrix(coordinates_cities_array, 532);
	
        print_matrix(distance_matrix_cities, 532);
	
	solution = random_permutation(532);
	random_search(distance_matrix_cities, solution, 532);

	
	return 0;
}

void random_search(double **distance_matrix, int *solution, int size)
{
	long start_mili, end_mili;
	struct timeval timecheck;
	int *random_solution;
	int counter = 0;

	gettimeofday(&timecheck, NULL);
        start_mili = (long) timecheck.tv_sec * 1000 + (long) timecheck.tv_usec / 1000;

	while (1)
	{
		gettimeofday(&timecheck, NULL);
	        end_mili = (long) timecheck.tv_sec * 1000 + (long) timecheck.tv_usec / 1000;
		if (end_mili -start_mili > 10000)
			break;
		

		random_solution = random_permutation(size);
		if (fitness(random_solution, distance_matrix, size) < fitness(solution, distance_matrix, size))
		{
			copy_solution(solution, random_solution, size);
			printf("Best fitness so far: %5.2lf\n", fitness(solution, distance_matrix, size));
		}
		free(random_solution);
		counter++;

	}
	
	printf("There were %d iterations\n", counter);

	return;
}

double fitness(int *solution, double **distance_matrix, int size)
{
	double fitness_value = 0;
	int i;

	for (i = 0; i < size - 1; i++)
	{
		fitness_value += distance_matrix[solution[i]][solution[i+1]];
	}
	fitness_value += distance_matrix[solution[size - 1]][solution[0]];

	return fitness_value;
}
	
void copy_solution(int *target, int *source, int size)
{
	int i;

	for (i = 0; i < size; i++)
		target[i] = source[i];

	return;
}
		
