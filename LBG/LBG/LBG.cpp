// LBG.cpp : Defines the entry point for the console application.

// Initialization
#include "stdafx.h"

#define K 8		  // Codebook size
#define P 12	  // LPC prediction order
#define S 200	  // line size
#define M 6340	  // Universe size
#define E 0.03	  // Epsilon
#define D 0.00001 // Delta: to compare distortion
int m;			  // Turns to find final codebook
float WEIGHTS[P] = {1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0};

void takeInput(float vector[][P])
{
	FILE *fin;
	int i, j;
	char line[S];

	// Open input file
	if (fopen_s(&fin, "C:\\Users\\panda\\Documents\\M Tech\\Sem 1\\Speech Processing\\Record Vector\\Universe.csv", "r"))
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
			if (!ptr)
				break;
			ptr++;
		}
		i++;
	}
	fclose(fin);
}

int makeCodebook(float codebook[][P], int size)
{
	int i, j;
	float newCodebook[K][P];

	if (!size)
	{
		for (j = 0; j < P; j++)
			codebook[0][j] = 0.0f;
		return 1;
	}

	for (i = 0; i < size; i++)
	{
		for (j = 0; j < P; j++)
		{
			newCodebook[2 * i][j] = codebook[i][j] + E;
			newCodebook[2 * i + 1][j] = codebook[i][j] - E;
		}
	}

	size *= 2;
	printf("After plus minus epsilon:\n\n");
	for (i = 0; i < size; i++)
		printf("%d\t\t", i);

	for (j = 0; j < P; j++)
	{
		printf("\n");
		for (i = 0; i < size; i++)
		{
			codebook[i][j] = newCodebook[i][j];
			printf("%f\t", codebook[i][j]);
		}
	}
	printf("\n\n\n");

	return size;
}

void storeOutput(float codebook[][P], int size)
{
	FILE *fout;
	int i, j;
	char input[S];

	sprintf(input, "C:\\Users\\panda\\Documents\\M Tech\\Sem 1\\Speech Processing\\Record Vector\\LBG_Output_%d.csv", size);
	if (fopen_s(&fout, input, "w"))
		return;

	for (i = 0; i < size; i++)
	{
		for (j = 0; j < P; j++)
		{
			fprintf(fout, "%f", codebook[i][j]);

			// comma separator
			if (j < P - 1)
				fprintf(fout, ",");
		}
		// new line after each row
		fprintf(fout, "\n");
	}

	fclose(fout);
}

float kMeansClustering(float vector[][P], float codebook[][P], int size)
{
	float d, dist, minDist, totalDist = 0.0f, distance[K] = {0.0f}, centroid[K][P] = {0.0f};
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

	printf("Turn %d.%d:-\n", size, m);
	for (j = 0; j < size; j++)
	{
		if (freq[j] == 0)
			continue;

		distance[j] /= (float)freq[j];
		printf("%d\t:\t%d\t&\t%f\n", j, freq[j], distance[j]);

		for (k = 0; k < P; k++)
			codebook[j][k] = centroid[j][k] / freq[j];
	}

	totalDist /= (float)M;
	printf("\nDistortion:\t\t\t%f\n\n\n", totalDist);

	return totalDist;
}

void LindeBuzoGray(float vector[][P], float codebook[][P], int size)
{
	int i, j;
	float prevDist = 0.0f, curDist = FLT_MAX;

	m = 0;
	while (abs(curDist - prevDist) > D)
	{
		m++;
		prevDist = curDist;
		curDist = kMeansClustering(vector, codebook, size);
	}

	for (i = 0; i < size; i++)
		printf("%d\t\t", i);
	for (j = 0; j < P; j++)
	{
		printf("\n");
		for (i = 0; i < size; i++)
			printf("%f\t", codebook[i][j]);
	}
	printf("\n\n\n");

	storeOutput(codebook, size);
}

int _tmain(int argc, _TCHAR *argv[])
{
	int size = 0;
	float vector[M][P], codebook[K][P];

	takeInput(vector);
	while (size < K)
	{
		size = makeCodebook(codebook, size);
		LindeBuzoGray(vector, codebook, size);
	}

	return 0;
}