// Similar to two node exchange delta calculation but this time returns the current fitness not the change of fitness
double calculate_shortened_fitness(int *solution, int i, int j, double **distance_matrix, int size, double previous_fitness);

// Random walk - change randomly two nodes and pick the best result
void random_walk(double **distance_matrix, int *solution, int size, int time_mili);

