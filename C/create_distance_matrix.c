// create_distance_matric.c - Implementing logic to create matrix that represents the distance between ith and jth city. 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include "read_and_allocate_data.h"
#include "create_distance_matrix.h"

/*
int main(void)
{
	char *file_path = "burma14.tsp";

	double **coordinates_cities_array = coordinates_cities(file_path);
	double **distance_matrix_cities = distance_matrix(coordinates_cities_array, 14);
	
	print_matrix(distance_matrix_cities, 14);


}
*/
 
double **distance_matrix(double **cities, int dimensions)
{
	int i, j;
	double ** distance_matrix_cities;
	
	distance_matrix_cities = initialize_matrix(dimensions);
	for (i = 0; i < dimensions; i++)
	{
		distance_matrix_cities[i][i] = 0.0;
		for (j = i+1; j < dimensions; j++)
		{
			distance_matrix_cities[i][j] = calculate_distance(cities[i][0],
									  cities[i][1],
									  cities[j][0],
									  cities[j][1]);
			distance_matrix_cities[j][i] = distance_matrix_cities[i][j];
		}
	
	}

	
	return distance_matrix_cities;
}


double calculate_distance(double x1, double y1, double x2, double y2)
{
	return sqrt( pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

double **initialize_matrix(int dimensions)
{

        double **distance_matrix_cities;
        int i, j;

        distance_matrix_cities = (double **)malloc(sizeof(double *) * dimensions);

        for (i = 0; i < dimensions; i++)
        {
                distance_matrix_cities[i] = (double *)malloc(sizeof(double) * dimensions);
        }
	
	
        return distance_matrix_cities;
}

void print_matrix(double **arr, int dimensions)
{
        int i, j;

        for (i = 0; i < dimensions; i++)
        {
		for (j = 0; j < dimensions; j++)
		{
	                printf("%5.2lf ", arr[i][j]);
		}
		printf("\n");
        }

        return;
}

