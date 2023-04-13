// steepest_local_search.c - Implementation of the steepest local search
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "random_permutation.h"
#include "read_and_allocate_data.h"
#include "create_distance_matrix.h"
#include "utils.h"

double acceptence_probability(double delta, double temperature);
double random_probability_01(void);
int acceptence(double probability);

void steepest_local_search(double **distance_matrix, int *solution, int size, long *iterations_done, long *evaluations_done, double acceptance_rate, double alpha, int markov);

void experiment_one_instance(char *file_name, int iterations, double *alphas, int *markov_lengths, double *ar, int size_alphas, int size_markov, int size_ar);

int main(int argc, char *argv[])
{
	if (argc < 3)
        {
                printf("Too little arguments\n");
                exit(1);
        }

	char *file_path = argv[2];
	
		
	int size, flag;

        double **coordinates_cities_array = coordinates_cities(file_path, &size, &flag);
       	/* 
	double **distance_matrix_cities = distance_matrix(coordinates_cities_array, size);
	
        //print_matrix(distance_matrix_cities, 532);
	srand(time(NULL));	
	solution = random_permutation(size);
	
	clock_t start = clock();
	steepest_local_search(distance_matrix_cities, solution, size, &iterations_done, &evaluations_done, 0.95, 0.8, 15);
	clock_t end = clock();

	printf("It took %lf seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

	double final_fitness = fitness(solution, distance_matrix_cities, size);
	
	printf("Final fitness: %lf, iterations done: %ld, evaluations done: %ld\n", final_fitness, iterations_done, evaluations_done);
	*/



	//******************* TESTING *****************************
	//You can add to the three arrays values to test 
	// Alphas should be between 0 and 1, (0.8 and 0.99 preferably)
	// Acceptance Rates should be between 0 and 1 (0.1 and 0.95 preferably)
	// Markov length should be between 1 and sizeof(instance)
	//
	// Tips:
	// It seems that small values of Markov length between 2% and 10% of instance size gives very good results while taking a lot less time to obtain the solution
	// You can also check how replacing the random_probability_01() function with a static value(most likely 0.5) will work, it seems to speed up the algorithm by 40%, but makes it deterministic
	//
	// Also you can change the stopping condition, right now we're stopping at temperature less than 0.1 and 100 iterations without improvement
	
		
	// After you experiment with different combinations of parameters and options just change the values in the 3 arrays in the for loop to one best value so each array will have the size of 1 and also don't forget to make changes in the saving conventions in the function experiment_one_instance
	int iterations = atoi(argv[1]);
	for (int i = 2; i < argc; i++)
	{

		char *file_path = argv[i];
		double **coordinates_cities_array = coordinates_cities(file_path, &size, &flag);
		double alphas[] = {0.92};
	        double acceptance_rates[] = {0.97};
        	int markov_length[] = {size*0.08};

	
	        int size_alphas = sizeof(alphas) / sizeof(double);
        	int size_markov = sizeof(markov_length) / sizeof(int);
        	int size_ar = sizeof(acceptance_rates) / sizeof(double);

		experiment_one_instance(file_path, iterations, alphas, markov_length, acceptance_rates, size_alphas, size_markov, size_ar);
	}

	return 0;
}

void experiment_one_instance(char *file_name, int iterations, double *alphas, int *markov_lengths, double *ar, int size_alphas, int size_markov, int size_ar)
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
	for (int x = 0; x < size_alphas; x++)
	{
		for (int y = 0; y < size_markov; y++)
		{
			for (int k = 0; k < size_ar; k++)
			{

				char simulated_annealing_save[128];
				strcpy(simulated_annealing_save, copy_file_path);
				// After the experiments comment out the next two lines
				sprintf(add_to_name, "_SA_alpha_%0.2lf_markov_%d_ar_%0.2lf.csv", alphas[x], markov_lengths[y], ar[k]);
				strcat(simulated_annealing_save, add_to_name);
				// After the experiments uncomment the next line
				// strcat(simulated_annealing_save, "_SA.csv");
				puts(simulated_annealing_save);
				for (int z = 0; z < iterations; z++)
				{
					counter++;

					shuffle(solution, size);
					fitness_initial_solution = fitness(solution, distance_matrix_cities, size);

					gettimeofday(&timecheck, NULL);
				        start_micro = (long) timecheck.tv_sec * 1000000 + (long) timecheck.tv_usec;
					clock_t start = clock();
					steepest_local_search(distance_matrix_cities, solution, size, &iterations_done, &evaluations_done, 0.5, alphas[x], markov_lengths[y]);
					clock_t end = clock();
			
					gettimeofday(&timecheck, NULL);
				        end_micro = (long) timecheck.tv_sec * 1000000 + (long) timecheck.tv_usec;
					time_micro_total = end_micro - start_micro;
					printf("It took %lf seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

					double final_fitness = fitness(solution, distance_matrix_cities, size);

					printf("Final fitness: %lf, iterations done: %ld, evaluations done: %ld\n\n\n", final_fitness, iterations_done, evaluations_done);

					save_as_csv(solution, fitness(solution, distance_matrix_cities, size), size, simulated_annealing_save, flag, time_micro_total, iterations_done, evaluations_done, fitness_initial_solution);
				}
			}
		}			
	}
	
	printf("Counter: %d\n", counter);

}




void steepest_local_search(double **distance_matrix, int *solution, int size, long *iterations_done, long *evaluations_done, double acceptance_rate, double alpha, int markov)
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
	
	
	double best_delta = 0;
	int best_i, best_j;
	double t = INFINITY;
	
	int stop_cond = 0;
	int last_amelioration = 0;
	int accepted_solutions = 0;

	double total_deltas = 0;
	int deteriorating_deltas = 0;

	while (1)
	{
		counter++;


		if (t < 0.1 || counter - last_amelioration > 100)
			break;
			

		if (counter == 2)
		{
			t = -(total_deltas / ((accepted_solutions + deteriorating_deltas) / 2)) / log(acceptance_rate);
		}
		if (counter % markov == 0)
			t = t * alpha;
		//printf("Iteration: %d; temperature: %lf; fitness: %lf / %lf; accepted solutions: %d/%d\n", counter, t, current_fitness, best_fitness, accepted_solutions, deteriorating_deltas);	

		double current_delta = 0;
		accepted_solutions = 0;
		total_deltas = 0;
		deteriorating_deltas = 0;

		for (int i = 0; i < size - 1; i++)
		{
			for (int j = i + 1; j < size; j++)
			{
				if ((current_delta = delta_two_nodes_exchange(solution, i, j, distance_matrix, size)) < best_delta || acceptence(acceptence_probability(current_delta, t)))

				{
					total_deltas += current_delta > 0 ? current_delta : 0;
					deteriorating_deltas += current_delta > 0 ? 1 : 0;

					current_fitness += current_delta;
					best_i = i;
					best_j = j;
					accepted_solutions++;
					swap(&solution[best_i], &solution[best_j]);	
				}

				if (current_fitness < best_fitness)
				{
					best_fitness = current_fitness;
					last_amelioration = counter;
				}

				if ((current_delta = delta_two_edge_exchange(solution, distance_matrix, size, i, j)) < best_delta || acceptence(acceptence_probability(current_delta, t)))

                                {
					total_deltas += current_delta > 0 ? current_delta : 0;
					deteriorating_deltas += current_delta > 0 ? 1 : 0;
					
					current_fitness += current_delta;
                                        best_i = i;
                                        best_j = j;
					accepted_solutions++;
					reverse_route(&solution[best_i+1], &solution[best_j]);
                                }
				count_evaluations += 2;
				
				if (current_fitness < best_fitness)
				{
					best_fitness = current_fitness;
					last_amelioration = counter;
				}
				
			}
		}


	}
	*iterations_done = counter; // count_evaluations
	*evaluations_done = count_evaluations;
	
	printf("Total steps: %ld; Total Evaluations: %ld\n", *iterations_done, *evaluations_done);

	double best_solution_found = fitness(solution, distance_matrix, size);


	return;
}

double acceptence_probability(double delta, double temperature)
{
	double probability;

	probability = exp(-delta / temperature);

	if (probability > 1){
		//printf("Greater than 1; probability: %lf; delta: %lf; temperature: %lf", probability, delta, temperature);
		probability = 1.0;
	}
	else if (probability < 0)
	{
		printf("Smaller than 0; probability: %lf; delta: %lf; temperature: %lf", probability, delta, temperature);
		probability = 0.0;
	}

	return probability;
}

double random_probability_01(void)
{
	double random_probability;
	random_probability = (double) rand() / (double) RAND_MAX;

	return random_probability;
}

int acceptence(double probability)
{
	if (probability > random_probability_01())
		return 1;
	return 0;
}

