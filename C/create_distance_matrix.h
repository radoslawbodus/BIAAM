// Create nxn distance matrix
double **distance_matrix(double **, int dimensions);

// Initialize nxn distance matrix
double **initialize_matrix(int dimensions);

// Initialize tabu matrix
int **initialize_tabu_matrix(int dimensions);

// Create a pseudo tabu dict
int **initialize_tabu_dict(int dimensions);

// Calculate distance between two points
double calculate_distance(double, double, double, double);

// Print 2d array
void print_matrix(double **arr, int dimensions);

