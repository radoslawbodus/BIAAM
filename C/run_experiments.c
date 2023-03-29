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
	//printf("%s\n", copy_file_path);
	/*	
	int string_length = strlen(file_path);
	
	char greedy_save[64];	
	slice(file_path, greedy_save, 0, string_length - 4);
	strcat(greedy_save, "_greedy.csv");
	printf("%s\n", greedy_save);
	
	char steepest_save[64];
	slice(file_path, steepest_save, 0, string_length - 4);
	strcat(steepest_save, "_steepest.csv");
	printf("%s\n", steepest_save);
	
	char random_search_save[64];
	slice(file_path, random_search_save, 0, string_length - 4);
	strcat(random_search_save, "_rs.csv");
	printf("%s\n", random_search_save);
	
	char random_walk_save[64];	
	slice(file_path, random_walk_save, 0, string_length - 4);
	strcat(random_walk_save, "_rw.csv");
	printf("%s\n", random_walk_save);
	
	printf("%s\n", steepest_save);	
	*/
	printf("\n");
	int i, size;
	int flag = 0;
	int time_mili;
	int flag_euc2d;
	//Deallocate
	//printf("xD: %d\n", size);
	double **coordinates_cities_array = coordinates_cities(file_path, &size, &flag_euc2d);
	if (flag_euc2d == 0)
	{
		//printf("EUC: %d\n", flag_euc2d); 
		deallocate_memory_2d(coordinates_cities_array, size);
		return;
	}
	//printf("FLAG: %d\n", flag_euc2d);
	//printf("xD: %d\n", size);
	//char *file_path = file_name;
	
	int string_length = strlen(file_path);
	
	char greedy_save[64];
	//slice(file_path, greedy_save, 0, string_length - 4);
	strcpy(greedy_save, copy_file_path);
	strcat(greedy_save, "_greedy.csv");
	//printf("%s\n", greedy_save);
	
	char steepest_save[64];
	//slice(file_path, steepest_save, 0, string_length - 4);
	strcpy(steepest_save, copy_file_path);
	strcat(steepest_save, "_steepest.csv");
	//printf("%s\n", steepest_save);
	
	char random_search_save[64];
	//slice(file_path, random_search_save, 0, string_length - 4);
	strcpy(random_search_save, copy_file_path);
	strcat(random_search_save, "_rs.csv");
	//printf("%s\n", random_search_save);
	
	char random_walk_save[64];	
	//slice(file_path, random_walk_save, 0, string_length - 4);
	strcpy(random_walk_save, copy_file_path);
	strcat(random_walk_save, "_rw.csv");
	//printf("%s\n", random_walk_save);
	


	printf("%d\n", flag_euc2d);
	//deallocate
        double **distance_matrix_cities = distance_matrix(coordinates_cities_array, size);	
	//deallocate
	int *solution = random_permutation(size);
	
	long start_micro, end_micro;
        struct timeval timecheck;
	long time_micro_greedy, time_micro_steepest;
	long iterations_done_rw, iterations_done_rs, iterations_done_steep, iterations_done_greed;
	srand(time(NULL));
	
	for (i = 0; i < iterations; i++)
	{
		if ((i+1) % 25 == 0)
		{
			printf("i: %d\n", i+1);
		}
		flag = 0;
		shuffle(solution, size);
		
		gettimeofday(&timecheck, NULL);
	        start_micro = (long) timecheck.tv_sec * 1000000 + (long) timecheck.tv_usec;
		
		greedy_local_search(distance_matrix_cities, solution, size, &iterations_done_greed);
	
		gettimeofday(&timecheck, NULL);
	        end_micro = (long) timecheck.tv_sec * 1000000 + (long) timecheck.tv_usec;
			
		time_micro_greedy = end_micro - start_micro;
		
		save_as_csv(solution, fitness(solution, distance_matrix_cities, size), size, greedy_save, flag, time_micro_greedy, iterations_done_greed);

		shuffle(solution, size);

		gettimeofday(&timecheck, NULL);
	        start_micro = (long) timecheck.tv_sec * 1000000 + (long) timecheck.tv_usec;
		
		steepest_local_search(distance_matrix_cities, solution, size, &iterations_done_steep);
		
		gettimeofday(&timecheck, NULL);
	        end_micro = (long) timecheck.tv_sec * 1000000 + (long) timecheck.tv_usec;
		
		time_micro_steepest = end_micro - start_micro;
		
		time_mili = time_micro_steepest / 1000;

		save_as_csv(solution, fitness(solution, distance_matrix_cities, size), size, steepest_save, flag, time_micro_steepest, iterations_done_steep);

		shuffle(solution, size);

		random_walk(distance_matrix_cities, solution, size, time_mili, &iterations_done_rw);
		save_as_csv(solution, fitness(solution, distance_matrix_cities, size), size, random_walk_save, flag, time_micro_steepest, iterations_done_rw);

		shuffle(solution, size);
		
		random_search(distance_matrix_cities, solution, size, time_mili, &iterations_done_rs);
		save_as_csv(solution, fitness(solution, distance_matrix_cities, size), size, random_search_save, flag, time_micro_steepest, iterations_done_rs);

	}

	deallocate_memory_2d(distance_matrix_cities, size);
	deallocate_memory_2d(coordinates_cities_array, size);
	free(solution);

}

