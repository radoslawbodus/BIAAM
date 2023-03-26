#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "read_and_allocate_data.h"

double **distance_matrix(double **, int dimensions);
double **initialize_matrix(int dimensions);
double calculate_distance(double, double, double, double);
void print_array(double **arr, int dimensions);

int main(void)
{
	char *file_path = "burma14.tsp";

	double **coordinates_cities_array = coordinates_cities(file_path);
	double **distance_matrix_cities = distance_matrix(coordinates_cities_array, 14);
	
	print_array(distance_matrix_cities, 14);


}

double **distance_matrix(double **cities, int dimensions)
{
	int i, j;
	double ** distance_matrix_cities;

	distance_matrix_cities = initialize_matrix(dimensions);

	for (i = 0; i < dimensions; i++)
	{
		distance_matrix_cities[i][i] = 0.0;
		for (j = i; j < dimensions; j++)
		{
			distance_matrix_cities[i][j] = calculate_distance(cities[i][0],
									  cities[i][1],
									  cities[j][0],
									  cities[j][1]);
		}
		printf("i: %d\n", i);
	
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
        int i;

        distance_matrix_cities = (double **)malloc(sizeof(double *) * dimensions);

        for (i = 0; i < dimensions; i++)
        {
                distance_matrix_cities[i] = (double *)malloc(sizeof(int) * dimensions);
        }

        return distance_matrix_cities;
}

void print_array(double **arr, int dimensions)
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

