// LBG.cpp : Defines the entry point for the console application.

// Initialization
#include "stdafx.h"

#define K 8				// Codebook size
#define P 12			// LPC prediction order
#define S 200			// line size
#define M 6340			// Universe size
#define E 0.03			// Epsilon

float WEIGHTS[P] = {1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0};


void takeInput(float vector[][P])
{
	FILE *fin;
	int i, j;
	char line[S];

	// Open input file
	if (fopen_s(&fin, "C:\\Users\\panda\\Documents\\M Tech\\Sem 1\\Speech Processing\\Record Vector\\Universe.csv", "r") != 0)
		return;

	// Read data
	i = 0;
	while (fgets(line, sizeof(line), fin) && i < M)
	{
		j = 0;
		char *ptr = line;
        while (sscanf_s(ptr, "%f", &vector[i][j]) && j < P)
		{
            j++;
            ptr = strchr(ptr, ',');
            if (!ptr) break;
            ptr++;
        }
		i++;
	}
	fclose(fin);
}


int makeCodebook(float codebook[][P], float newCodebook[][P], int size)
{
	int i, j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < P; j++)
		{
			newCodebook[2*i][j] = codebook[i][j] + E;
			newCodebook[2*i + 1][j] = codebook[i][j] - E;
		}
	}

    for (i = 0; i < 2*size; i++)
		for (j = 0; j < P; j++)
			codebook[i][j] = newCodebook[i][j];
    
    return 2*size;
}


void kMeansClustering(float vector[][P], float codebook[][P], float newCodebook[][P], int size)
{
	float dist, d, minDist, totalDist = 0.0f, distance[K] = {0.0f}, centroid[K][P] = {0.0f};
    int i, j, k, group[M], freq[K] = {0};

	// Tokhura's Distance
	for (i = 0; i < M; i++)
	{
		minDist = FLT_MAX;
		for (j = 0; j < size; j++)
		{
			dist = 0.0f;
			for (k = 0; k < P; k++) 
            {
                d = codebook[j][k] - vector[i][k];
                dist += WEIGHTS[k] * d * d;
            }

			if (minDist > dist)
			{
				minDist = dist;
				group[i] = j;
			}
		}

		// printf("%d -> %d;\t", i, group[i]);
		j = group[i];
		distance[j] += minDist;
		freq[j]++;
		totalDist += minDist;
		for (k = 0; k < P; k++) 
			centroid[j][k] += vector[i][k];
	}

	printf("Turn %d:-\n", size);
	for (j = 0; j < size; j++)
	{
		if (freq[j] == 0) 
			continue;

		distance[j] /= (float) freq[j];
		printf("%d\t:\t%d\t&\t%f\n", j, freq[j], distance[j]);

		for (k = 0; k < P; k++) 
			newCodebook[j][k] = centroid[j][k] / freq[j];
	}

	totalDist /= (float) M;
	printf("\nDistortion:\t\t\t%f\n\n\n", totalDist);

	for (j = 0; j < size; j++)
		printf("%d\t\t", j);
	for (k = 0; k < P; k++)
	{
		printf("\n");
		for (j = 0; j < size; j++) 
        {
			printf("%f\t", codebook[j][k]);
            codebook[j][k] = newCodebook[j][k];
        }
	}
	printf("\n\n");
	for (j = 0; j < size; j++)
		printf("%d\t\t", j);
	for (k = 0; k < P; k++)
	{
		printf("\n");
		for (j = 0; j < size; j++) 
			printf("%f\t", codebook[j][k]);
	}
	printf("\n\n\n");
}


int _tmain(int argc, _TCHAR* argv[])
{
	int j, k, size = 1;
	float vector[M][P], codebook[K][P], newCodebook[K][P];

	takeInput(vector);
	for (j = 0; j < P; j++)
		codebook[0][j] = vector[0][j];
	kMeansClustering(vector, codebook, newCodebook, size);

	while (size < K)
	{
        size = makeCodebook(codebook, newCodebook, size);
		kMeansClustering(vector, codebook, newCodebook, size);
	}

	return 0;
}