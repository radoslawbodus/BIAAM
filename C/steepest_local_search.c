#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "random_permutation.h"
#include "read_and_allocate_data.h"
#include "create_distance_matrix.h"
#include "utils.h"

/*
double fitness(int *solution, double **distance_matrix, int size);
void steepest_local_search(double **distance_matrix, int *solution, int size);
void copy_solution(int *target, int *source, int size);
void swap_here(int *a, int *b);
double calculate_shortened_fitness(int *solution, int i, int j, double **distance_matrix, int size, double previous_fitness);
void print_array_here(int *arr, int size);
double calculate_fitness_edge_exchange(int *solution, double **distance_matrix, int size, int i, int j);
void reverse_sub(int *start, int *end);
*/

void steepest_local_search(double **distance_matrix, int *solution, int size);

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

void steepest_local_search(double **distance_matrix, int *solution, int size)
{
	long start_mili, end_mili;
	struct timeval timecheck;
	int *random_solution = random_permutation(532);
	copy_solution(random_solution, solution, size);
	
	int counter = 0;
	double current_fitness = fitness(random_solution, distance_matrix, size);
	double previous_fitness = current_fitness;
	double best_fitness = current_fitness;
	int i, j;
	int type = 0;	
	
	gettimeofday(&timecheck, NULL);
        
	start_mili = (long) timecheck.tv_sec * 1000 + (long) timecheck.tv_usec / 1000;
	
	srand(start_mili);
	int stop_cond = 0;
	while (1)
	{
		gettimeofday(&timecheck, NULL);
	        end_mili = (long) timecheck.tv_sec * 1000 + (long) timecheck.tv_usec / 1000;
		if (end_mili -start_mili > 10000)
			break;
			
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
					//printf("Best delta: %lf\n", best_delta);
					//printf("Best solution so far: %lf\n", fitness(solution, distance_matrix, size));
				}
				if ((current_delta = delta_two_edge_exchange(solution, distance_matrix, size, i, j)) < best_delta)

                                {
                                        best_delta = current_delta;
                                        best_i = i;
                                        best_j = j;
                                        type = 1;
                                        //printf("Nice\n");
                                        break;
                                }

			}
		}
		if (best_delta < 0)
                {
                        if (type == 0)
                                swap(&solution[best_i], &solution[best_j]);
                        else if (type == 1)
                                reverse_route(&solution[best_i+1], &solution[best_j]);
                        //printf("Best Fitness: %lf Best delta: %lf type: %d, (i: %d, j: %d)\n", fitness(solution, distance_matrix, size), best_delta, type, best_i, best_j);
                        //print_array_here(solution, size);
                }

		else
		{
			break;
		}

		counter++;

	}
	
	printf("There were %d iterations\n", counter);
	double best_solution_found = fitness(solution, distance_matrix, size);

	printf("Best solution found: %lf\n", best_solution_found);

	return;
}


/*
void reverse_sub(int *start, int *end)
{
        int i = 0;
        double temp;
        int middle = (end - start + 1) / 2;
        //printf("Middle: %d  ", middle);
        while (i < middle)
        {
                temp = *(start + i); 
                *(start + i) = *(end - i);
                *(end - i) = temp;
                i++;
        }
}


double calculate_fitness_edge_exchange(int *solution, double **distance_matrix, int size, int i, int j)
{
        if ((j - i == 1) || (i == 0 && j == size - 1))
                return 0;

        int jp1 = (j == size - 1 ? 0 : j + 1);
        double difference_minus = 0, difference_plus = 0;

        difference_minus += (distance_matrix[solution[i]][solution[i+1]] +
                             distance_matrix[solution[j]][solution[jp1]]);
        difference_plus += (distance_matrix[solution[i]][solution[j]] +
                            distance_matrix[solution[i+1]][solution[jp1]]);

        return (difference_plus - difference_minus);
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
	
	return (difference_plus - difference_minus);
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
