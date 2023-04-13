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


void reduce_tenure(int **tabu_matrix, int **tabu_list, int size, int candidates);
void recreate_tabu_dict(int **tabu_dict, int *solution, int size);
double **create_candidate_moves(int candidates);
double **create_candidate_matrix(int size);
void prepare_candidate_moves(double **candidate_moves, int candidates);
void recreate_candidate_moves(double **candidate_list, double **distance_matrix, double **candidate_matrix, int *solution, int candidates, int size);
void tabu_search(double **distance_matrix, int *solution, int size, long *iterations_done, long *evaluations_done, int tenure, int candidats);
void experiment_one_instance(char *file_name, int iterations, int *tenure, int *candidates_sizes, int size_tenures, int size_candidates);
int **create_tabu_list(int size);
int calculate_non_zero(int **matrix, int size);
void reset_matrix(int **matrix, int size);

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
	long tt = time(NULL);
        srand(123);      
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
                int tenures[] = {size/2, size, size*2, size*4, size*8};
                int candidate_sizes[] = {size/10};


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
                               
				//steepest_local_search(distance_matrix_cities, solution, size, &iterations_done, &evaluations_done);
                                      
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
	//int **tabu_dict = initialize_tabu_dict(size);

	candidates = tenure;
	int **tabu_list = create_tabu_list(candidates);

	//double **candidate_moves = create_candidate_moves(candidates);
	//double **candidate_matrix = create_candidate_matrix(size);

	//recreate_candidate_moves(candidate_moves, distance_matrix, candidate_matrix, solution, candidates, size);

	//recreate_tabu_dict(tabu_dict, solution, size);
	
	int index;
	int add_to_tabu = 0;
	int tabu_rotation = 0;
	int last_upgrade = 0;
	int max_no_upgrades = size * 2;
	while (1)
	{

		gettimeofday(&timecheck, NULL);     
		start_micro = (long) timecheck.tv_sec * 1000000 + (long) timecheck.tv_usec;

		counter++;

		
		index = -1;
		double best_delta = INFINITY;
		int best_i, best_j;
		double current_delta = 0;

		
		for (int i = 0; i < size - 1; i++)
		{
			for (int j = i + 1; j < size; j++)
			{

				count_evaluations+=2;
				if ((((current_delta = delta_two_nodes_exchange(solution, i, j, distance_matrix, size)) < best_delta) && (tabu_matrix[solution[i]][solution[j]] == 0) ) || ((1) && (tabu_matrix[solution[i]][solution[j]] != 0) && ((current_fitness + current_delta) < best_fitness - 0.001)))
				{
					if ((tabu_matrix[solution[i]][solution[j]] != 0) && ((current_fitness + current_delta) < best_fitness - 0.001))
						add_to_tabu = 0;
					else
						add_to_tabu = 1;

					best_delta = current_delta;
        	                        best_i = i;
	                                best_j = j;
					type = 0;
					index = i;
				}
				if (((((current_delta = delta_two_edge_exchange(solution, distance_matrix, size, i, j)) < best_delta) && (tabu_matrix[solution[i]][solution[j]] == 0)) || ((1) && (tabu_matrix[solution[i]][solution[j]] != 0) && ((current_fitness + current_delta) < best_fitness - 0.001))) && (abs(i - j) != 1) )
				{
					if ((tabu_matrix[solution[i]][solution[j]] != 0) && ((current_fitness + current_delta) < best_fitness - 0.001))
                                                add_to_tabu = 0;
                                        else
                                                add_to_tabu = 1;
					
					best_delta = current_delta;
                                        best_i = i;
                                        best_j = j;
                                        type = 1;
					index = i;
				}
			}
		}

		

		gettimeofday(&timecheck, NULL);
		end_micro = (long) timecheck.tv_sec * 1000000 + (long) timecheck.tv_usec;
		time_micro_total = end_micro - start_micro;
		//printf("First time in micro_sec: %ld\n", time_micro_total);


		//TODO: Create tabu list instead of candidate_moves
		reduce_tenure(tabu_matrix, tabu_list, size, candidates);
                
		if (index != -1)
		{

			
			if (best_i > best_j)
			{	
				int temp = best_i;
				best_i = best_j;
				best_j = temp;
			}

			if (type == 0)
			{
				tabu_matrix[solution[best_i]][solution[best_j]] = tabu_tenure;
				swap(&solution[best_i], &solution[best_j]);
			}
			else if (type == 1)
			{
				tabu_matrix[solution[best_i]][solution[best_j]] = tabu_tenure;
				reverse_route(&solution[best_i+1], &solution[best_j]);
			}

			current_fitness += best_delta;
			if (current_fitness < best_fitness)
			{
				best_fitness = current_fitness;
				copy_solution(best_solution, solution, size);
				last_upgrade = counter;
				//printf("Upgrade was made at: %d (%lf)\n", last_upgrade, best_fitness);
				//printf("XDDDD; Fitness: %lf\n", best_fitness);
			}
			
			if (add_to_tabu)
			{
				tabu_list[tabu_rotation % candidates][0] = solution[best_i];
				tabu_list[tabu_rotation % candidates][1] = solution[best_j];
				tabu_rotation++;
				tabu_matrix[solution[best_i]][solution[best_j]] = tabu_tenure;
			}
			else
			{
				//printf("Aspiration was made\n");
			}
		}
		else
		{
			int a = 0;
			//printf("WTF");
		}

		
		//printf("Counter: %d; Fitness: %lf (%lf); index: %d best delta: %lf\n", counter, fitness(solution, distance_matrix, size), current_fitness, index, best_delta);	
		
		//TODO: Check if necessary
		
		
		if (counter - last_upgrade > max_no_upgrades)
			break;
	

		
		gettimeofday(&timecheck, NULL);
		end_micro = (long) timecheck.tv_sec * 1000000 + (long) timecheck.tv_usec;
		time_micro_total = end_micro - start_micro;
		//printf("End time in micro_sec: %ld\n", time_micro_total);

	}
	*iterations_done = counter; // count_evaluations
	*evaluations_done = count_evaluations;
	
	copy_solution(solution, best_solution, size);
	
	int total = calculate_non_zero(tabu_matrix, size);
	printf("TOTAL: %d\n", total);

	double best_solution_found = fitness(solution, distance_matrix, size);


	return;
}

void reset_matrix(int **matrix, int size)
{
        int i, j;

        for (i = 0; i < size-1; i++)
        {
                for (j = i + 1; j < size; j++)
                {
                        matrix[i][j] = 0;
                }
        }

        return;
}


int calculate_non_zero(int **matrix, int size)
{
        int total = 0;
        int i, j;

        for (i = 0; i < size-1; i++)
        {
                for (j = i + 1; j < size; j++)
                {
                        if (matrix[i][j] > 0)
                                total++;
                }
        }

        return total;
}


int **create_tabu_list(int size)
{
	int **tabu_list;
        int i, j;

        tabu_list = (int **)malloc(sizeof(int *) * size);

        for (i = 0; i < size; i++)
        {
                tabu_list[i] = (int *)malloc(sizeof(int) * 2);
		tabu_list[i][0] = 0;
        }


        return tabu_list;
}

void reduce_tenure(int **tabu_matrix, int **tabu_list, int size, int tabu_size)
{
        
        int i;

        for (i = 0; i < tabu_size; i++)
        {
                if ( tabu_matrix[tabu_list[i][0]][tabu_list[i][1]] != 0 )
                {
                        tabu_matrix[tabu_list[i][0]][tabu_list[i][1]] -= 1;
                }
        }



        return;
}

/* OLD
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
*/

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


			
