// time_measure.c - A Logic for calculating the wall and cpu time
// TODO: Make the function passed to a 'repeat_func' variadic (if possible)
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <stdarg.h>
#include "time_measure.h"

/*
int main(void)
{
	repeat_func(count_to_n, 100, 3, 100, 2, 3);
}
*/

void repeat_func(void (*f)(int), int iterations, int argc, ...)
{
	va_list list;
	int n;

	clock_t begin, end;
	double time_total, time_average;
	
	long start_mili, end_mili;
	long time_total_mili, time_average_mili;
	struct timeval timecheck;
	
	va_start(list, argc);
	n = va_arg(list, int);
	va_end(list);

	gettimeofday(&timecheck, NULL);
	start_mili = (long) timecheck.tv_sec * 1000 + (long) timecheck.tv_usec / 1000;

	begin = clock();
	
	for (int i = 0; i < iterations; i++)
		f(n);

	end = clock();

	gettimeofday(&timecheck, NULL);
	end_mili = (long) timecheck.tv_sec * 1000 + (long) timecheck.tv_usec / 1000;


	time_total = (double)(end - begin) / CLOCKS_PER_SEC;
	time_average = time_total / iterations;

	time_total_mili = end_mili - start_mili;
	time_average_mili = time_total_mili / iterations;
	
	printf("CPU TIME: Time total: %lf, Time average: %lf\n", time_total, time_average);
	printf("WALL TIME: Time total: %ld, Time average: %ld\n", time_total_mili, time_average_mili);

	return;
}

void count_to_n(int n)
{
	int i;
	printf("WTF\n");
	for (i = 0; i < n; i++)
		printf("Hello man\n");
		
	return;
}

