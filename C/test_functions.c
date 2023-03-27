#include <stdio.h>
#include "random_permutation.h"
#include "time_measure.h"
#include "create_distance_matrix.h"

int main(void)
{
	printf("Test random_permutation:\n");
	repeat_func(random_permutation_time_test, 100, 1, 100);
	
	printf("\n");
	printf("Test matrix creation:\n");
	repeat_func(distance_matrix_time_test, 100, 1, 8);

}
