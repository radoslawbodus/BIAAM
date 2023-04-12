# Experiments with simulated annealing

Compile command: gcc -O3 simulated\_annealing.c random\_permutation.c read\_and\_allocate\_data.c create\_distance\_matrix.c utils.c -lm -o simulated\_annealing.out

Invoke this command in the parent directory and cp the executable here

It works similar to the run\_experiments.out executable, but now only 2 arguments are allowed, first is the number of iterations and the second one is the file path to the tsp instance

