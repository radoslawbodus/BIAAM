#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double **coordinates_cities(char *);
double **initialize_coordinates(int dimensions);
void print_array_2d(double **, int dimensions);

/*
int main(int argc, char *argv[])
{
	double **coordinates;
	
	printf("%d\n", argc);

	if (argc < 2)
	{
		printf("Error\n");
		exit(EXIT_FAILURE);
	}
	
	printf("%s\n", argv[1]);


	coordinates = coordinates_cities(argv[1]);
	
	return 0;
}
*/

double **coordinates_cities(char * file_name)
{
	double **coordinates;

	FILE *fp;
	int read;

	char name[32];
	int dimensions;
	
	int dummy, dummy2, previous, current;
	int i = 0, successful_reads = 0;

	char ch, line[32];
	int line_length;

	if ((fp = fopen(file_name, "r")) == NULL)
	{
		printf("Can't open %s\n", file_name);
		exit(EXIT_FAILURE);
	}
	
	read = fscanf(fp, "NAME :%s \n", name);

	if (read)
		printf("name is: %s\n", name);
	
	while ((read = fscanf(fp, "DIMENSION : %d \n", &dimensions)) == 0)
	{
		while (fgetc(fp) != '\n')
			continue;
	}

	if (read)
	{
		printf("Dimension: %d\n", dimensions);
		coordinates = initialize_coordinates(dimensions);
	}
	else
	{
		printf("Couldn't read the dimensions, abord :(\n");
		fclose(fp);
		exit(EXIT_FAILURE);
	}


	while (fgets(line, 32, fp) != NULL)
	{
		line_length = strlen(line);
		if (line[line_length-1] == '\n')
		{
			if ((read = sscanf(line, "%d %lf %lf %d", &dummy, &coordinates[i][0], &coordinates[i][1], &dummy2) >= 3))
			{
				current = read;
				if (current != previous || i == 0)
				{
					if (i == 0)
					{
						i++;
						successful_reads++;
						previous = read;
					}
					continue;		
				}
				i++;
				successful_reads++;
				previous = read;
			}
		}
		else
		{
			while (fgetc(fp) != '\n')
				continue;
		}
	}

	printf("successful reads: %d\n", successful_reads);
	
	if (successful_reads != dimensions)
	{	
		printf("Format not supported (yet) :(\n");
		fclose(fp);
		exit(EXIT_FAILURE);
	}
	
	print_array_2d(coordinates, dimensions);

	fclose(fp);

	return coordinates;
}

double **initialize_coordinates(int dimensions)
{
	double **coordinates;
	int i;

	coordinates = (double **)malloc(sizeof(double *) * dimensions);
	
	for (i = 0; i < dimensions; i++)
	{
		coordinates[i] = (double *)malloc(sizeof(int) * 2);
	}
	
	return coordinates;
}	

void print_array_2d(double **arr, int dimensions)
{
	int i;

	for (i = 0; i < dimensions; i++)
	{
		printf("%d: x=%lf, y=%lf\n", i, arr[i][0], arr[i][1]);
	}

	return;
}



