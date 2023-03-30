#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include "create_distance_matrix.h"
#include "random_permutation.h"
#include "read_and_allocate_data.h"
#include "steepest_local_search.h"
#include "greedy_local_search.h"
#include "random_walk.h"
#include "random_search.h"
#include "nearest_neighbor.h"
#include "utils.h"

void slice(const char *str, char *result, size_t start, size_t end);
void one_instance_tsp(char *file_name, int iterations);

int main(int argc, char *argv[])
{

	if (argc < 3)
	{
		printf("Daj argument byczku\n");
		exit(1);
	}
	
	int i;
	
	for (i = 2; i < argc; i++)
	{
		printf("%d: %s\n", i-1, argv[i]);
		one_instance_tsp(argv[i], atoi(argv[1]));
	}
	
	
	return 0;
}

void one_instance_tsp(char *file_name, int iterations)
{
	
	char *file_path = file_name;
	char copy_file_path[strlen(file_path) + 1];
	strcpy(copy_file_path, file_path);
	copy_file_path[strlen(copy_file_path) - 4] = 0;
	
	int i, size;
	int flag = 0;
	int time_mili;
	int flag_euc2d;
	double **coordinates_cities_array = coordinates_cities(file_path, &size, &flag_euc2d);
	if (flag_euc2d == 0)
	{
		deallocate_memory_2d(coordinates_cities_array, size);
		return;
	}
	
	int string_length = strlen(file_path);
	
	char greedy_save[64];
	strcpy(greedy_save, copy_file_path);
	strcat(greedy_save, "_greedy.csv");
	
	char steepest_save[64];
	strcpy(steepest_save, copy_file_path);
	strcat(steepest_save, "_steepest.csv");
	
	char random_search_save[64];
	strcpy(random_search_save, copy_file_path);
	strcat(random_search_save, "_rs.csv");
	
	char random_walk_save[64];	
	strcpy(random_walk_save, copy_file_path);
	strcat(random_walk_save, "_rw.csv");
	
	char heuristic_save[64];	
	strcpy(heuristic_save, copy_file_path);
	strcat(heuristic_save, "_h.csv");


        double **distance_matrix_cities = distance_matrix(coordinates_cities_array, size);	
	int *solution = random_permutation(size);
	
	long start_micro, end_micro;
        struct timeval timecheck;
	long time_micro_greedy, time_micro_steepest;
	long iterations_done_rw, iterations_done_rs, iterations_done_steep, iterations_done_greed, iterations_done_h;
	long evaluations_done_greed, evaluations_done_steep;
	double fitness_initial_solution = 0;
	srand(time(NULL));
	
	for (i = 0; i < iterations; i++)
	{
		if ((i+1) % 25 == 0)
		{
			printf("i: %d\n", i+1);
		}
		flag = 0;
		shuffle(solution, size);
		
		
		fitness_initial_solution = fitness(solution, distance_matrix_cities, size);
		
		gettimeofday(&timecheck, NULL);
	        start_micro = (long) timecheck.tv_sec * 1000000 + (long) timecheck.tv_usec;
		
		greedy_local_search(distance_matrix_cities, solution, size, &iterations_done_greed, &evaluations_done_greed);
		gettimeofday(&timecheck, NULL);
	        end_micro = (long) timecheck.tv_sec * 1000000 + (long) timecheck.tv_usec;
			
		time_micro_greedy = end_micro - start_micro;
		
		save_as_csv(solution, fitness(solution, distance_matrix_cities, size), size, greedy_save, flag, time_micro_greedy, iterations_done_greed, evaluations_done_greed, fitness_initial_solution);
		
		shuffle(solution, size);

		fitness_initial_solution = fitness(solution, distance_matrix_cities, size);
		
		gettimeofday(&timecheck, NULL);
	        start_micro = (long) timecheck.tv_sec * 1000000 + (long) timecheck.tv_usec;
		
		steepest_local_search(distance_matrix_cities, solution, size, &iterations_done_steep, &evaluations_done_steep);
		
		gettimeofday(&timecheck, NULL);
	        end_micro = (long) timecheck.tv_sec * 1000000 + (long) timecheck.tv_usec;
		
		time_micro_steepest = end_micro - start_micro;
		
		time_mili = time_micro_steepest / 1000;
		if ((i + 1) % 10 == 0)
			printf("Time in miliseconds (Steepest): %d\n", time_mili);
		
		save_as_csv(solution, fitness(solution, distance_matrix_cities, size), size, steepest_save, flag, time_micro_steepest, iterations_done_steep, evaluations_done_steep, fitness_initial_solution);
		shuffle(solution, size);
		
		fitness_initial_solution = fitness(solution, distance_matrix_cities, size);
		
		random_walk(distance_matrix_cities, solution, size, time_micro_steepest, &iterations_done_rw);
		save_as_csv(solution, fitness(solution, distance_matrix_cities, size), size, random_walk_save, flag, time_micro_steepest, iterations_done_rw, iterations_done_rw, fitness_initial_solution);

		shuffle(solution, size);
		
		fitness_initial_solution = fitness(solution, distance_matrix_cities, size);
		
		random_search(distance_matrix_cities, solution, size, time_mili, &iterations_done_rs);
		save_as_csv(solution, fitness(solution, distance_matrix_cities, size), size, random_search_save, flag, time_micro_steepest, iterations_done_rs, iterations_done_rs, fitness_initial_solution);

		shuffle(solution, size);
		
		fitness_initial_solution = fitness(solution, distance_matrix_cities, size);

		gettimeofday(&timecheck, NULL);
                start_micro = (long) timecheck.tv_sec * 1000000 + (long) timecheck.tv_usec;

		nearest_neighbor(distance_matrix_cities, solution, size, &iterations_done_h);

		gettimeofday(&timecheck, NULL);
                end_micro = (long) timecheck.tv_sec * 1000000 + (long) timecheck.tv_usec;


		save_as_csv(solution, fitness(solution, distance_matrix_cities, size), size, heuristic_save, flag, end_micro - start_micro, iterations_done_h, iterations_done_h, fitness_initial_solution);


	}

	printf("\n");
	
	deallocate_memory_2d(distance_matrix_cities, size);
	deallocate_memory_2d(coordinates_cities_array, size);
	free(solution);

}

