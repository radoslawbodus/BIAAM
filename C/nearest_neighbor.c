// nearest_neighbor.c - Implements nearest neighbor algorithm for tsp
#include <stdio.h>
#include "utils.h"
#include "nearest_neighbor.h"


void nearest_neighbor(double **distance_matrix, int *solution, int size, long *iterations_done)
{
	int counter = 0;
	int i, j;
	double nearest_neighbor_value;
	int nearest_neighbor_index;
	double total_distance = 0;

	for (i = 0; i < size-1; i++)
	{
		nearest_neighbor_value = 1000000;
		for (j = i+1; j < size; j++)
		{
			if (distance_matrix[solution[i]][solution[j]] < nearest_neighbor_value)
			{
				nearest_neighbor_value = distance_matrix[solution[i]][solution[j]];
				nearest_neighbor_index = j;
			}
			counter++;
		}
		swap(&solution[i+1], &solution[nearest_neighbor_index]);
		total_distance += distance_matrix[solution[i]][solution[i+1]];
	}
	*iterations_done = counter;
		
	return;
}
