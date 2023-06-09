// Calculate the fitness of the solution i.e. The cost of the hamiltionian cycle
double fitness(int *solution, double **distance_matrix, int size);

// Copy a solution to another array
void copy_solution(int *target, int *source, int size);

// Swap two values (in this case, those are nodes)
void swap(int *a, int *b);

// Calculate the change in solution fitness after two nodes exchange
double delta_two_nodes_exchange(int *solution, int i, int j, double **distance_matrix, int size);
 
// Calculate the change in solution fitness after two edge exchange
double delta_two_edge_exchange(int *solution, double **distance_matrix, int size, int i, int j);

// Print 1d array
void print_1d_array(int *arr, int size);

// Reverse sub route in solution
void reverse_route(int *start, int *end);

// Saves solution to csv
void save_as_csv(int *solution, double fitness_value, int size, char *name, int flag, long time, long iterations_done, long evaluations_done, double fitness_is);

// Deallocate memory
void deallocate_memory_2d(double **arr, int n);

// Deallocate tabu matrix
void deallocate_tabu_matrix(int **tabu_arr, int n);

