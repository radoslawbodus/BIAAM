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
	clock_t start = clock();
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
	clock_t end = clock();
	printf("%lf\n", (double)(end - start) / CLOCKS_PER_SEC * 1000);

	
	return distance_matrix_cities;
}
/*
void distance_matrix_time_test(int i)
{
	char *instances[9] = {
	"burma14.tsp", "bays29.tsp", "berlin52.tsp",
	"eli76.tsp", "gr137.tsp", "gr229.tsp",
	"a280.tsp", "att532.tsp", "d657.tsp"
	};
	
	int size;

	const int sizes[9] = {14, 29, 52, 76, 137, 229, 280, 532, 657};
	
	char *file_path = instances[i];

	double **coordinates_cities_array = coordinates_cities(file_path, &size);

	//clock_t begin = clock();
	double **distance_matrix_cities = distance_matrix(coordinates_cities_array, sizes[i]);
	//clock_t end = clock();
	//printf("CPU TIME: %lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
	free(coordinates_cities_array);
	free(distance_matrix_cities);
}
*/

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
	
	/*
	for (i = 0; i < dimensions; i++)
		for (j = 0; j < dimensions; j++)
			distance_matrix_cities[i][j] = 0;
	*/
	
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

