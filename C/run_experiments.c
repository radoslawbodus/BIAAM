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


int main(int argc, char *argv[])
{

	if (argc < 2)
	{
		printf("Daj argument byczku\n");
		exit(1);
	}
	
	char *file_path = argv[1];
	
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
	
		save_as_csv(solution, fitness(solution, distance_matrix_cities, size), size, greedy_save, flag);

		shuffle(solution, size);

		steepest_local_search(distance_matrix_cities, solution, size);
		save_as_csv(solution, fitness(solution, distance_matrix_cities, size), size, steepest_save, flag);

		shuffle(solution, size);

		random_walk(distance_matrix_cities, solution, size, time_mili);
		save_as_csv(solution, fitness(solution, distance_matrix_cities, size), size, random_walk_save, flag);

		shuffle(solution, size);
		
		random_search(distance_matrix_cities, solution, size, time_mili);
		save_as_csv(solution, fitness(solution, distance_matrix_cities, size), size, random_search_save, flag);

	}

	return 0;
}


void slice(const char *str, char *result, size_t start, size_t end)
{
    strncpy(result, str + start, end - start);
}
