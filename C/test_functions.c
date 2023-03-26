#include <stdio.h>
#include "random_permutation.h"
#include "time_measure.h"

int main(void)
{
	repeat_func(random_permutation_time_test, 1000, 1, 100);
}
