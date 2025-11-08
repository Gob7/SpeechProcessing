// HiddenMarkovModel.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#define N 5	  // Number of STATES
#define M 32  // Number of OBSERVATION/STATE
#define T 85  // Length of OBSERVATION SEQUENCE
#define S 300 // line size

void readAMatrix(double A[][N])
{
	FILE *fin;
	int i, j;
	char line[S];

	// Open input file
	if (fopen_s(&fin, "C:\\Users\\panda\\Documents\\M Tech\\Sem 1\\Speech Processing\\Record Vector\\HMM_A_MATRIX.csv", "r"))
		return;

	// Read data
	i = 0;
	while (fgets(line, sizeof(line), fin) && i < N)
	{
		j = 0;
		char *ptr = line;
		while (sscanf_s(ptr, "%lf", &A[i][j]) && j < N)
		{
			// printf("%lf\t", A[i][j]);
			j++;
			ptr = strchr(ptr, ',');
			if (!ptr)
				break;
			ptr++;
		}
		// printf("\n");
		i++;
	}
	fclose(fin);
}

void readBMatrix(double B[][M])
{
	FILE *fin;
	int i, j;
	char line[S];

	// Open input file
	if (fopen_s(&fin, "C:\\Users\\panda\\Documents\\M Tech\\Sem 1\\Speech Processing\\Record Vector\\HMM_B_MATRIX.csv", "r"))
		return;

	// Read data
	i = 0;
	while (fgets(line, sizeof(line), fin) && i < N)
	{
		j = 0;
		char *ptr = line;
		while (sscanf_s(ptr, "%lf", &B[i][j]) && j < M)
		{
			// printf("%lf\t", B[i][j]);
			j++;
			ptr = strchr(ptr, ',');
			if (!ptr)
				break;
			ptr++;
		}
		// printf("\n\n\n");
		i++;
	}
	fclose(fin);
}

void readPMatrix(double P[])
{
	FILE *fin;
	int i, j;
	char line[S];

	// Open input file
	if (fopen_s(&fin, "C:\\Users\\panda\\Documents\\M Tech\\Sem 1\\Speech Processing\\Record Vector\\HMM_P_MATRIX.csv", "r"))
		return;

	// Read data
	i = 0;
	while (fgets(line, sizeof(line), fin) && i < 1)
	{
		j = 0;
		char *ptr = line;
		while (sscanf_s(ptr, "%lf", &P[j]) && j < N)
		{
			// printf("%lf\t", P[j]);
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

void readOMatrix(int O[])
{
	FILE *fin;
	int i, j;
	char line[S];

	// Open input file
	if (fopen_s(&fin, "C:\\Users\\panda\\Documents\\M Tech\\Sem 1\\Speech Processing\\Record Vector\\HMM_OBSERVATION_SEQUENCE.csv", "r"))
		return;

	// Read data
	i = 0;
	while (fgets(line, sizeof(line), fin) && i < 1)
	{
		j = 0;
		char *ptr = line;
		while (sscanf_s(ptr, "%d", &O[j]) && j < T)
		{
			// printf("%d,\t", O[j]);
			j++;
			ptr = strchr(ptr, ',');
			if (!ptr)
				break;
			ptr++;
		}
		i++;
	}
	fclose(fin);

	for (i = 0; i < T; i++)
		O[i] -= 1;
}

double forwardProcedure(double A[][N], double B[][M], double P[], int O[])
{
	double a[T][N], s, p = 0.0;
	int i, j, t;

	// Initialization
	// printf("\nO[0] = %d\n", O[0]);
	for (i = 0; i < N; i++)
	{
		a[0][i] = P[i] * B[i][O[0]];
		// printf("a[0][%d]\t= %lf =\t%lf\t*\t%lf\n", i, a[0][i], P[i], B[i][O[0]]);
	}

	// Induction
	for (t = 1; t < T; t++)
	{
		for (j = 0; j < N; j++)
		{
			s = 0.0;
			for (i = 0; i < N; i++)
				s += a[t - 1][i] * A[i][j];
			a[t][j] = s * B[j][O[t]];
		}
	}

	// Termination
	for (i = 0; i < N; i++)
	{
		p += a[T - 1][i];
		// printf("a[%d][%d] = %lf\n", T - 1, i, a[T - 1][i]);
	}

	return p;
}

int _tmain(int argc, _TCHAR *argv[])
{
	double A[N][N], B[N][M], P[N], p;
	int O[T];

	readAMatrix(A);
	readBMatrix(B);
	readPMatrix(P);
	readOMatrix(O);

	p = forwardProcedure(A, B, P, O);
	printf("\n\nP(O | y) = %lf\n", p);

	return 0;
}
