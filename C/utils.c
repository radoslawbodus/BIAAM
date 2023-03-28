#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils.h"

double fitness(int *solution, double **distance_matrix, int size)
{
        double fitness_value = 0;
        int i;

        for (i = 0; i < size - 1; i++)
        {
                fitness_value += distance_matrix[solution[i]][solution[i+1]];
        }
        fitness_value += distance_matrix[solution[size - 1]][solution[0]];

        return fitness_value;
}

void copy_solution(int *target, int *source, int size)
{
        int i;

        for (i = 0; i < size; i++)
                target[i] = source[i];

        return;
}

void swap(int *a, int *b)
{
        int temp;
        temp = *a;
        *a = *b;
        *b = temp;

        return;
}

double delta_two_nodes_exchange(int *solution, int i, int j, double **distance_matrix, int size)
{
        double difference_minus = 0, difference_plus = 0;

        int temp;
        if (i > j)
        {
                temp = j;
                j = i;
                i = temp;
        }

        int im1 = (i == 0 ? size - 1 : i - 1);
        int ip1 = (i == size - 1 ? 0 : i + 1);

        int jm1 = (j == 0 ? size - 1 : j - 1);
        int jp1 = (j == size - 1 ? 0 : j + 1);

        difference_minus += (distance_matrix[ solution[im1] ][ solution[i] ] +
                            distance_matrix[ solution[i] ][ solution[ip1] ] +
                            distance_matrix[ solution[jm1] ][ solution[j] ] +
                            distance_matrix[ solution[j] ][ solution[jp1] ]);

        if  (i == 0 && j == size - 1)
        {
                difference_plus += (distance_matrix[ solution[jm1] ][solution[i]] +
                            distance_matrix[solution[i]][ solution[j] ] +
                            distance_matrix[ solution[i] ][ solution[j] ] +
                            distance_matrix[ solution[j] ][ solution[ip1]]);
        }
        else if (j - i == 1)
        {
                difference_plus += (distance_matrix[ solution[im1] ][solution[j]] +
                            distance_matrix[solution[j]][ solution[i] ] +
                            distance_matrix[ solution[j] ][ solution[i] ] +
                            distance_matrix[ solution[i] ][ solution[jp1]]);
        }
        else {
                difference_plus += (distance_matrix[ solution[im1] ][solution[j]] +
                                    distance_matrix[solution[j]][ solution[ip1] ] +
                                    distance_matrix[ solution[jm1] ][ solution[i] ] +
                                    distance_matrix[ solution[i] ][ solution[jp1]]);
        }


        return (difference_plus - difference_minus);
}

double delta_two_edge_exchange(int *solution, double **distance_matrix, int size, int i, int j)
{
        if ((j - i == 1) || (i == 0 && j == size - 1))
                return 0;

        int jp1 = (j == size - 1 ? 0 : j + 1);
        double difference_minus = 0, difference_plus = 0;

        difference_minus += (distance_matrix[solution[i]][solution[i+1]] +
                             distance_matrix[solution[j]][solution[jp1]]);
        difference_plus += (distance_matrix[solution[i]][solution[j]] +
                            distance_matrix[solution[i+1]][solution[jp1]]);

        return (difference_plus - difference_minus);
}


void print_1d_array(int *arr, int size)
{
        int i;
        for (i = 0; i < size; i++)
                printf("%d ", arr[i]);
        printf("\n");

        return;
}

void reverse_route(int *start, int *end)
{
        int i = 0;
        double temp;
        int middle = (end - start + 1) / 2;
        //printf("Middle: %d  ", middle);
        while (i < middle)
        {
                temp = *(start + i);
                *(start + i) = *(end - i);
                *(end - i) = temp;
                i++;
        }
}

void save_as_csv(int *solution, int size, char *name, int flag)
{
        FILE *fp;
        int i;
        char *string;

        if (flag)
                string = "";
        else
                string = "\n";

        if (access(name, F_OK) == 0)
        {
                fp = fopen(name, "a");
        }
        else
        {
                fp = fopen(name, "w");
        }
        if (fp == NULL)
        {
                printf("Error!");
                exit(1);
        }


        for (i = 0; i < size - 1; i++)
        {
                fprintf(fp, "%d ", solution[i]);
        }

        fprintf(fp, "%d", solution[size - 1]);

        fprintf(fp, "%s", string);

        fclose(fp);

        return;
}

