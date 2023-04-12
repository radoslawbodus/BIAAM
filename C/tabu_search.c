// steepest_local_search.c - Implementation of the steepest local search
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "steepest_local_search.h"
#include "random_permutation.h"
#include "read_and_allocate_data.h"
#include "create_distance_matrix.h"
#include "utils.h"


void reduce_tenure(int **tabu_matrix, double **candidate_moves, int size, int candidates);
void recreate_tabu_dict(int **tabu_dict, int *solution, int size);
double **create_candidate_moves(int candidates);
double **create_candidate_matrix(int size);
void prepare_candidate_moves(double **candidate_moves, int candidates);
void recreate_candidate_moves(double **candidate_list, double **distance_matrix, double **candidate_matrix, int *solution, int candidates, int size);
void tabu_search(double **distance_matrix, int *solution, int size, long *iterations_done, long *evaluations_done, int tenure, int candidats);
void experiment_one_instance(char *file_name, int iterations, int *tenure, int *candidates_sizes, int size_tenures, int size_candidates);
int main(int argc, char *argv[])
{
	if (argc < 3)
        {
                printf("Too little arguments\n");
                exit(1);
        }

	char *file_path = argv[2];

	int *solution;
	int size, flag;

	long iterations_done, evaluations_done;

        double **coordinates_cities_array = coordinates_cities(file_path, &size, &flag);
        double **distance_matrix_cities = distance_matrix(coordinates_cities_array, size);
        
        //print_matrix(distance_matrix_cities, 532);
        srand(time(NULL));      
        solution = random_permutation(size);
        steepest_local_search(distance_matrix_cities, solution, size, &iterations_done, &evaluations_done);

	printf("Fitness initial solution: %lf\n", fitness(solution, distance_matrix_cities, size));
        clock_t start = clock();
        tabu_search(distance_matrix_cities, solution, size, &iterations_done, &evaluations_done, 20, 10);
        clock_t end = clock();

        printf("It took %lf seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

        double final_fitness = fitness(solution, distance_matrix_cities, size);
        
        printf("Final fitness: %lf, iterations done: %ld, evaluations done: %ld\n", final_fitness, iterations_done, evaluations_done);
	
	int iterations = atoi(argv[1]);
        for (int i = 2; i < argc; i++)
        {

                char *file_path = argv[i];
                double **coordinates_cities_array = coordinates_cities(file_path, &size, &flag);
                int tenures[] = {size/10, size/5, size/2};
                int candidate_sizes[] = {size/10, size/5, size/2};


                int size_tenures = sizeof(tenures) / sizeof(int);
                int size_candidates = sizeof(candidate_sizes) / sizeof(int);

                experiment_one_instance(file_path, iterations, tenures, candidate_sizes, size_tenures, size_candidates);
        }


	return 0;
}

void experiment_one_instance(char *file_name, int iterations, int *tenures, int *candidate_sizes, int size_tenures, int size_candidates)
{
	char *file_path = file_name;
        char copy_file_path[strlen(file_path) + 1];
        strcpy(copy_file_path, file_path);
        copy_file_path[strlen(copy_file_path) - 4] = 0;

        long start_micro, end_micro;
        long time_micro_total;
        struct timeval timecheck;

        double fitness_initial_solution = 0;
        long iterations_done, evaluations_done;
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

	double **distance_matrix_cities = distance_matrix(coordinates_cities_array, size);

        int *solution = random_permutation(size);

        int string_length = strlen(file_path);

        char add_to_name[128];
        double a = INFINITY;
        int counter = 0;
	
	for (int x = 0; x < size_tenures; x++)
	{
		for (int y = 0; y < size_candidates; y++)
		{
			char tabu_search_save[128];
                        strcpy(tabu_search_save, copy_file_path);
                       	// After the experiments comment out the next two lines
                      	sprintf(add_to_name, "_TS_tenure_%d_candidates_%d.csv", tenures[x], candidate_sizes[y]);
                     	strcat(tabu_search_save, add_to_name);
                    	// After the experiments uncomment the next line
                      	// strcat(tabu_search_save, "_TS.csv");
                      	puts(tabu_search_save);

			for (int z = 0; z < iterations; z++)
                        {
                              	counter++;

                              	shuffle(solution, size);
                               
				steepest_local_search(distance_matrix_cities, solution, size, &iterations_done, &evaluations_done);
                                      
				fitness_initial_solution = fitness(solution, distance_matrix_cities, size);
				gettimeofday(&timecheck, NULL);
                                  
			  	start_micro = (long) timecheck.tv_sec * 1000000 + (long) timecheck.tv_usec;
                                        
				clock_t start = clock();                
				tabu_search(distance_matrix_cities, solution, size, &iterations_done, &evaluations_done, tenures[x], candidate_sizes[y]);
				clock_t end = clock();

                                
				gettimeofday(&timecheck, NULL);
                        
				end_micro = (long) timecheck.tv_sec * 1000000 + (long) timecheck.tv_usec;
                
				time_micro_total = end_micro - start_micro;
        
				printf("It took %lf seconds\n", (double)(end - start) / CLOCKS_PER_SEC);


				double final_fitness = fitness(solution, distance_matrix_cities, size);


				printf("Initial fitness: %lf; Final fitness: %lf, iterations done: %ld, evaluations done: %ld\n\n\n", fitness_initial_solution, final_fitness, iterations_done, evaluations_done);


				save_as_csv(solution, fitness(solution, distance_matrix_cities, size), size, tabu_search_save, flag, time_micro_total, iterations_done, evaluations_done, fitness_initial_solution);
                                }
		}
	}



}


void tabu_search(double **distance_matrix, int *solution, int size, long *iterations_done, long *evaluations_done, int tenure, int candidates)
{
	long start_micro, end_micro;
        long time_micro_total;
        struct timeval timecheck;

	int counter = 0;
	int count_evaluations = 0;
	double current_fitness = fitness(solution, distance_matrix, size);
	double previous_fitness = current_fitness;
	double best_fitness = current_fitness;
	int *best_solution = random_permutation(size);
       	copy_solution(best_solution, solution, size);
	
	int i, j;
	int type = 0;

	int tabu_tenure = tenure;
	int **tabu_matrix = initialize_tabu_matrix(size);
	int **tabu_dict = initialize_tabu_dict(size);


	double **candidate_moves = create_candidate_moves(candidates);
	double **candidate_matrix = create_candidate_matrix(size);

	recreate_candidate_moves(candidate_moves, distance_matrix, candidate_matrix, solution, candidates, size);

	recreate_tabu_dict(tabu_dict, solution, size);
	
	int index;
	while (1)
	{

		gettimeofday(&timecheck, NULL);     
		start_micro = (long) timecheck.tv_sec * 1000000 + (long) timecheck.tv_usec;

		counter++;
		index = -1;
		double best_delta = INFINITY;
		int best_i, best_j;
		double current_delta = 0;


		for (int i = 0; i < candidates; i++)
		{

			count_evaluations+=2;
			
			int proper_i = (int) candidate_moves[i][0];
			int proper_j = (int) candidate_moves[i][1];
			int tabu_i = proper_i, tabu_j = proper_j;
			
			if (tabu_i > tabu_j)
                        {
                                int temp = tabu_i;
                                tabu_i = tabu_j;
                                tabu_j = temp;
                        }

			proper_i = tabu_dict[proper_i][1];
			proper_j = tabu_dict[proper_j][1];

			if (proper_i > proper_j)
			{
				int temp = proper_i;
				proper_i = proper_j;
				proper_j = temp;
			}
			

			if ((((current_delta = delta_two_nodes_exchange(solution, proper_i, proper_j, distance_matrix, size)) < best_delta) && (tabu_matrix[tabu_i][tabu_j] == 0) ) || ((1) && (tabu_matrix[tabu_i][tabu_j] != 0) && ((current_fitness + current_delta) < best_fitness)))
			{

				//printf("XD; %lf\n", current_fitness + current_delta);
				//printf("index: %d; i: %d (%d); j: %d (%d); best_delta: %lf\n", i, proper_i, (int) candidate_moves[i][0], proper_j, (int) candidate_moves[i][1], current_delta);
				best_delta = current_delta;
				best_i = proper_i;
				best_j = proper_j;
				type = 0;
				index = i;
			}
			if (((((current_delta = delta_two_edge_exchange(solution, distance_matrix, size, proper_i, proper_j)) < best_delta) && (tabu_matrix[tabu_i][tabu_j] == 0)) || ((1) && (tabu_matrix[tabu_i][tabu_j] != 0) && ((current_fitness + current_delta) < best_fitness))) && (abs(proper_i - proper_j) != 1) )
			{
				//printf("XD1\n");	
				//printf("index: %d; i: %d (%d); j: %d (%d); best_delta: %lf\n", i, proper_i, (int) candidate_moves[i][0], proper_j, (int) candidate_moves[i][1], current_delta);
				best_delta = current_delta;
				best_i = proper_i;
				best_j = proper_j;
				type = 1;
				index = i;
			}
		}
		

		gettimeofday(&timecheck, NULL);
		end_micro = (long) timecheck.tv_sec * 1000000 + (long) timecheck.tv_usec;
		time_micro_total = end_micro - start_micro;
		//printf("First time in micro_sec: %ld\n", time_micro_total);

		reduce_tenure(tabu_matrix, candidate_moves, size, candidates);
                if (index != -1)
		{
			if (type == 0)
			{
				if (best_i > best_j)
				{
					int temp = best_i;
					best_i = best_j;
					best_j = temp;
				}
				tabu_matrix[(int) candidate_moves[index][0]][(int) candidate_moves[index][1]] = tabu_tenure;
				swap(&solution[best_i], &solution[best_j]);
			}
			else if (type == 1)
			{

				if (best_i > best_j)
				{
					int temp = best_i;
					best_i = best_j;
					best_j = temp;
				}
				//printf("XDDDDDDDDDDDDDDDDDDD\n");
				tabu_matrix[(int) candidate_moves[index][0]][(int) candidate_moves[index][1]] = tabu_tenure;

				reverse_route(&solution[best_i+1], &solution[best_j]);
			}

			current_fitness += best_delta;
			if (current_fitness < best_fitness)
			{
				best_fitness = current_fitness;
				copy_solution(best_solution, solution, size);
				//printf("XDDDD; Fitness: %lf\n", best_fitness);
			}
		}
		else
		{
			int a = 0;
			//printf("WTF");
		}
		
		//printf("Counter: %d; Fitness: %lf (%lf); index: %d best delta: %lf\n", counter, fitness(solution, distance_matrix, size), current_fitness, index, best_delta);	
		
		recreate_tabu_dict(tabu_dict, solution, size);
		
		if (counter % tenure == 0)
			recreate_candidate_moves(candidate_moves, distance_matrix, candidate_matrix, solution, candidates, size);
		if (counter > 10000)
			break;
	

		
		gettimeofday(&timecheck, NULL);
		end_micro = (long) timecheck.tv_sec * 1000000 + (long) timecheck.tv_usec;
		time_micro_total = end_micro - start_micro;
		//printf("End time in micro_sec: %ld\n", time_micro_total);

	}
	*iterations_done = counter; // count_evaluations
	*evaluations_done = count_evaluations;
	
	copy_solution(solution, best_solution, size);

	double best_solution_found = fitness(solution, distance_matrix, size);


	return;
}

void reduce_tenure(int **tabu_matrix, double **candidate_moves, int size, int candidates)
{
	
	int i;

	for (i = 0; i < candidates; i++)
	{
		if ( tabu_matrix[(int) candidate_moves[i][0]][(int) candidate_moves[i][1]] != 0 )
		{
			tabu_matrix[(int) candidate_moves[i][0]][(int) candidate_moves[i][1]] -= 1;
		}
	}



	return;
}


void recreate_tabu_dict(int **tabu_dict, int *solution, int size)
{
	int i;

	for (i = 0; i < size; i++)
	{
		tabu_dict[solution[i]][1] = i;
	}

	return;
}

double **create_candidate_moves(int candidates)
{
	double **candidate_list;
        int i, j;

        candidate_list = (double **)malloc(sizeof(double *) * candidates);

        for (i = 0; i < candidates; i++)
        {
                candidate_list[i] = (double *)malloc(sizeof(double) * 3);
                candidate_list[i][2] = INFINITY;
        }


        return candidate_list;
}

double **create_candidate_matrix(int size)
{
	double **candidate_matrix;
	int i, j;

	candidate_matrix = (double **)malloc(sizeof(double *) * size);

	for (i = 0; i < size; i++)
	{
		candidate_matrix[i] = (double *)malloc(sizeof(double) * size);
		candidate_matrix[i][0] = INFINITY;
	}

	return candidate_matrix;
}

void prepare_candidate_moves(double **candidate_moves, int candidates)
{
	int i;
	
	for (i = 0; i < candidates; i++)
		candidate_moves[i][2] = INFINITY;

	return;
}


void recreate_candidate_moves(double **candidate_list, double **distance_matrix, double **candidate_matrix, int *solution, int candidates, int size)
{
	int i, j, k;
	double delta_nodes, delta_edges;
	double smaller_delta;

	prepare_candidate_moves(candidate_list, candidates);

	for (i = 0; i < size - 1; i++)
	{
		for (j = i + 1; j < size; j++)
		{
			delta_nodes = delta_two_nodes_exchange(solution, i, j, distance_matrix, size);
			delta_edges = delta_two_edge_exchange(solution, distance_matrix, size, i, j);
			smaller_delta = delta_nodes > delta_edges ? delta_edges : delta_nodes;
			candidate_matrix[solution[i]][solution[j]] = smaller_delta;
			candidate_matrix[solution[j]][solution[i]] = smaller_delta;
		}
	}

	int best_i, best_j;
	double best_delta;

	for (k = 0; k < candidates; k++)
	{
		best_delta = INFINITY;
		for (i = 0; i < size - 1; i++)
		{
			for (j = i + 1; j < size; j++)
			{
				if (candidate_matrix[i][j] < best_delta)
				{
					best_delta = candidate_matrix[i][j];
					best_i = i;
					best_j = j;
				}
			}
		}
		candidate_list[k][0] = (double) best_i;
		candidate_list[k][1] = (double) best_j;
		candidate_list[k][2] = best_delta;

		candidate_matrix[best_i][best_j] = INFINITY;
		candidate_matrix[best_j][best_i] = INFINITY;
	}



	return;	
}


			
