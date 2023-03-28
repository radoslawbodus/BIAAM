# BIAAM

## Algorithms written in C and C++ (possibly with Rust in near future)
Let's go 

Compile command:
gcc -O3 run_experiments.c create_distance_matrix.c random_permutation.c read_and_allocate_data.c steepest_local_search.c greedy_local_search.c random_walk.c random_search.c utils.c -lm -o run_experiments.out


Use Command:
./run_experiments.out burma14.tsp (the argument is the instance of tsp, only one argument allowed right now)


Only TSP instances that have coordinates of the cities are allowed so no matricies (at least for now).

