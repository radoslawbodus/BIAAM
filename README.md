# BIAAM

## Algorithms written in C

Compile command:
gcc -O3 run\_experiments.c create\_distance\_matrix.c random\_permutation.c read\_and\_allocate\_data.c steepest\_local\_search.c greedy\_local\_search.c random\_walk.c random\_search.c nearest\_neighbor.c utils.c -lm -o run\_experiments.out

Command use:
./run\_experiments.out i[number greater than 0 which will indicate how many iterations of the algorithms are to be done] instance.tsp [instances of tsp]

Sample use:
./run\_experiments.out 10 burm14.tsp a280.tsp

This command will produce n (the number of instances with EUC\_2D type of distance) by 5 (5 algorithms) .csv files storing the tours and additional information in form of initial random solution, best found solution, time taken in microseconds, numbers of steps, numbers of evaluations. Tours and any additional information was obtained by running the given algorithms. 


