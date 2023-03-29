#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "random_permutation.h"
#include "read_and_allocate_data.h"
#include "create_distance_matrix.h"
#include "random_walk.h"
#include "utils.h"

/*
double fitness(int *solution, double **distance_matrix, int size);
void random_walk(double **distance_matrix, int *solution, int size);
void copy_solution(int *target, int *source, int size);
void swap_here(int *a, int *b);
double calculate_shortened_fitness(int *solution, int i, int j, double **distance_matrix, int size, double previous_fitness);
void print_array_here(int *arr, int size);
*/

double calculate_shortened_fitness(int *solution, int i, int j, double **distance_matrix, int size, double previous_fitness);

//void random_walk(double **distance_matrix, int *solution, int size);

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

void random_walk(double **distance_matrix, int *solution, int size, int time_mili, long *iterations_done)
{
	long start_mili, end_mili;
	struct timeval timecheck;
	int *random_solution = random_permutation(size);
	copy_solution(random_solution, solution, size);
	
	int counter = 0;
	double current_fitness = fitness(random_solution, distance_matrix, size);
	double current_delta = 0;
	double best_fitness = current_fitness;
	int i, j;
	
	
	gettimeofday(&timecheck, NULL);
        
	start_mili = (long) timecheck.tv_sec * 1000 + (long) timecheck.tv_usec / 1000;
	
	//srand(start_mili);
	int stop_cond = 0;	
	while (1)
	{
		gettimeofday(&timecheck, NULL);
	        end_mili = (long) timecheck.tv_sec * 1000 + (long) timecheck.tv_usec / 1000;
		if (end_mili -start_mili > time_mili)
			break;
		
		
		i = rand() % size;
		j = rand() % (size - 1);

		if (i == j)
			j = size - 1;
		
		if ((current_fitness = calculate_shortened_fitness(random_solution, i, j, distance_matrix, size, current_fitness)) < best_fitness)
		
		//if ((current_delta = delta_two_nodes_exchange(random_solution, i, j, distance_matrix, size)) < 0)
		{
			swap(&random_solution[i], &random_solution[j]);
			copy_solution(solution, random_solution, size);
			best_fitness = fitness(solution, distance_matrix, size);
			
			//printf("Best fitness so far: %5.2lf ; %5.2f\n", fitness(solution, distance_matrix, size), best_fitness);
		}
		else
		{
			swap(&random_solution[i], &random_solution[j]);
		}

		counter++;

	}
	*iterations_done = counter;
	//printf("There were %d iterations\n", counter);
	//printf("Best fitness: %5.2lf\n", fitness(solution, distance_matrix, size));
	
	return;
}

void swap_here(int *a, int *b)
{
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;

	return;
}



double calculate_shortened_fitness(int *solution, int i, int j, double **distance_matrix, int size, double previous_fitness)
{
	double current_fitness = previous_fitness;
	double difference_minus = 0, difference_plus = 0;
	
	int temp;
	if (i > j)
	{
		temp = j;
		j = i;
		i = temp;
	}

	int im1 = (i == 0 ? size - 1 : i - 1);
	int ip1 = (i == size - 1 ? 0 : i + 1);

	int jm1 = (j == 0 ? size - 1 : j - 1);
	int jp1 = (j == size - 1 ? 0 : j + 1);
	
	difference_minus += (distance_matrix[ solution[im1] ][ solution[i] ] +
			    distance_matrix[ solution[i] ][ solution[ip1] ] +
			    distance_matrix[ solution[jm1] ][ solution[j] ] +
			    distance_matrix[ solution[j] ][ solution[jp1] ]);
	
	if  (i == 0 && j == size - 1)
	{
		difference_plus += (distance_matrix[ solution[jm1] ][solution[i]] +
                            distance_matrix[solution[i]][ solution[j] ] +
                            distance_matrix[ solution[i] ][ solution[j] ] +
                            distance_matrix[ solution[j] ][ solution[ip1]]);
	}
	else if (j - i == 1)
	{
		difference_plus += (distance_matrix[ solution[im1] ][solution[j]] +
                            distance_matrix[solution[j]][ solution[i] ] +
                            distance_matrix[ solution[j] ][ solution[i] ] +
                            distance_matrix[ solution[i] ][ solution[jp1]]);
	}
	else {
		difference_plus += (distance_matrix[ solution[im1] ][solution[j]] + 
				    distance_matrix[solution[j]][ solution[ip1] ] +
				    distance_matrix[ solution[jm1] ][ solution[i] ] +
				    distance_matrix[ solution[i] ][ solution[jp1]]);
	}

	current_fitness += (difference_plus - difference_minus);
	
	
	double fitness_in_the_function = fitness(solution, distance_matrix, size);
	
	return current_fitness;
}


/*
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

void print_array_here(int *arr, int size)
{
	int i;
	for (i = 0; i < size; i++)
		printf("%d ", arr[i]);
	printf("\n");
	
	return;
}

void copy_solution(int *target, int *source, int size)
{
	int i;

	for (i = 0; i < size; i++)
		target[i] = source[i];

	return;
}
*/	
