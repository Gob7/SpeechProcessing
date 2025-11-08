// HiddenMarkovModel.cpp : Defines the entry point for the console application.

// Constants
#include "stdafx.h"
#define N 5	  // Number of STATES
#define M 32  // Number of OBSERVATION/STATE
#define T 85  // Length of OBSERVATION SEQUENCE
#define S 300 // line size

// Global Variables
double A[N][N], B[N][M], P[N];
int O[T];

void printMatrix()
{
	int i, j;
	float s[N] = {0.0};

	printf("\n\n\n\t");
	for (i = 0; i < N; i++)
		printf("%d\t\t", i);

	printf("\n\nP:\t");
	for (i = 0; i < N; i++)
		printf("%lf\t", P[i]);

	printf("\n\nA:");
	for (i = 0; i < N; i++)
	{
		printf("\n%d ->\t", i);
		for (j = 0; j < N; j++)
			printf("%lf\t", A[i][j]);
	}

	printf("\n\nB:");
	for (j = 0; j < M; j++)
	{
		printf("\n%d ->\t", j);
		for (i = 0; i < N; i++)
		{
			s[i] += B[i][j];
			printf("%lf\t", B[i][j]);
		}
	}
	printf("\n\nsum ->\t");
	for (i = 0; i < N; i++)
		printf("%lf\t", s[i]);
}

void readAMatrix()
{
	FILE *fin;
	int i, j;
	char line[S];

	// Open input file
	if (fopen_s(&fin, "C:\\Users\\panda\\Documents\\M Tech\\Sem 1\\Speech Processing\\Record Vector\\HMM_MATRIX_A.csv", "r"))
		return;

	// Read data
	i = 0;
	while (fgets(line, sizeof(line), fin) && i < N)
	{
		j = 0;
		char *ptr = line;
		while (sscanf_s(ptr, "%lf", &A[i][j]) && j < N)
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

void readBMatrix()
{
	FILE *fin;
	int i, j;
	char line[S];

	// Open input file
	if (fopen_s(&fin, "C:\\Users\\panda\\Documents\\M Tech\\Sem 1\\Speech Processing\\Record Vector\\HMM_MATRIX_B.csv", "r"))
		return;

	// Read data
	i = 0;
	while (fgets(line, sizeof(line), fin) && i < N)
	{
		j = 0;
		char *ptr = line;
		while (sscanf_s(ptr, "%lf", &B[i][j]) && j < M)
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

void readPMatrix()
{
	FILE *fin;
	int i, j;
	char line[S];

	// Open input file
	if (fopen_s(&fin, "C:\\Users\\panda\\Documents\\M Tech\\Sem 1\\Speech Processing\\Record Vector\\HMM_MATRIX_P.csv", "r"))
		return;

	// Read data
	i = 0;
	while (fgets(line, sizeof(line), fin) && i < 1)
	{
		j = 0;
		char *ptr = line;
		while (sscanf_s(ptr, "%lf", &P[j]) && j < N)
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

void makeUnbiasedMatrix()
{
	double a = 1.0 / N, b = 1.0 / M;
	int i, j;

	for (i = 0; i < N; i++)
	{
		P[i] = a;
		for (j = 0; j < N; j++)
			A[i][j] = a;
		for (j = 0; j < M; j++)
			B[i][j] = b;
	}
}

void readOMatrix()
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
		--O[i];
}

void finalMatrix()
{
	FILE *fout;
	int i, j;

	if (fopen_s(&fout, "C:\\Users\\panda\\Documents\\M Tech\\Sem 1\\Speech Processing\\Record Vector\\HMM_FINAL_P.csv", "w"))
		return;
	for (i = 0; i < 1; i++)
	{
		for (j = 0; j < N; j++)
		{
			fprintf(fout, "%f", P[j]);

			// comma separator
			if (j < N - 1)
				fprintf(fout, ",");
		}
		// new line after each row
		fprintf(fout, "\n");
	}

	if (fopen_s(&fout, "C:\\Users\\panda\\Documents\\M Tech\\Sem 1\\Speech Processing\\Record Vector\\HMM_FINAL_A.csv", "w"))
		return;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			fprintf(fout, "%f", A[i][j]);

			// comma separator
			if (j < N - 1)
				fprintf(fout, ",");
		}
		// new line after each row
		fprintf(fout, "\n");
	}

	if (fopen_s(&fout, "C:\\Users\\panda\\Documents\\M Tech\\Sem 1\\Speech Processing\\Record Vector\\HMM_FINAL_B.csv", "w"))
		return;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
		{
			fprintf(fout, "%f", B[i][j]);

			// comma separator
			if (j < M - 1)
				fprintf(fout, ",");
		}
		// new line after each row
		fprintf(fout, "\n");
	}

	fclose(fout);
}

double forwardProcedure(double alpha[][N])
{
	double s, probability = 0.0;
	int i, j, t;

	// Initialization
	for (i = 0; i < N; i++)
		alpha[0][i] = P[i] * B[i][O[0]];

	// Induction
	for (t = 1; t < T; t++)
	{
		for (j = 0; j < N; j++)
		{
			s = 0.0;
			for (i = 0; i < N; i++)
				s += alpha[t - 1][i] * A[i][j];

			alpha[t][j] = s * B[j][O[t]];
		}
	}

	// Termination
	for (i = 0; i < N; i++)
		probability += alpha[T - 1][i];

	return probability;

	// printf("alpha[0][%d] = %lf\t= %lf * %lf\n", i, alpha[0][i], P[i], B[i][O[0]]);
	// printf("alpha[%d][%d] = %lf\n", T - 1, i, alpha[T - 1][i]);
}

void backwardProcedure(double beta[][N])
{
	int i, j, t;

	// Initialization
	for (i = 0; i < N; i++)
		beta[T - 1][i] = 1.0;

	// Induction
	for (t = T - 2; t >= 0; t--)
	{
		for (i = 0; i < N; i++)
		{
			beta[t][i] = 0.0;
			for (j = 0; j < N; j++)
				beta[t][i] += A[i][j] * B[j][O[t + 1]] * beta[t + 1][j];
		}
	}

	// printf("beta[%d][%d] = %lf\n", T - 1, i, beta[T - 1][i], P[i], B[i][O[0]]);
	// for (i = 0; i < N; i++)
	// 	printf("beta[0][%d] = %lf\n", i, beta[0][i]);
}

double viterbiProcedure(double delta[][N], int psi[][N], int q_star[])
{
	double p_star, v, maxValue;
	int i, j, t, maxState;

	// Initialization
	for (i = 0; i < N; i++)
	{
		delta[0][i] = P[i] * B[i][O[0]];
		psi[0][i] = 0;
	}

	// Induction
	for (t = 1; t < T; t++)
	{
		for (j = 0; j < N; j++)
		{
			maxValue = 0.0;
			maxState = 0;

			for (i = 0; i < N; i++)
			{
				v = delta[t - 1][i] * A[i][j];
				if (v > maxValue)
				{
					maxValue = v;
					maxState = i;
				}
			}

			delta[t][j] = maxValue * B[j][O[t]];
			psi[t][j] = maxState;
		}
	}

	// Termination
	maxValue = 0.0;
	maxState = 0;

	for (i = 0; i < N; i++)
	{
		v = delta[T - 1][i];
		if (v > maxValue)
		{
			maxValue = v;
			maxState = i;
		}
	}

	p_star = maxValue;
	q_star[T - 1] = maxState;

	// Backtracking
	for (t = T - 2; t >= 0; t--)
		q_star[t] = psi[t + 1][q_star[t + 1]];

	return p_star;

	// printf("delta[0][%d] = %lf\t= %lf * %lf \npsi[0][%d] = %lf\n", i, delta[0][i], P[i], B[i][O[0]], i, psi[0][i]);
	// for (i = 0; i < N; i++)
	// 	printf("delta[%d][%d] = %lf \npsi[%d][%d] = %lf\n", T - 1, i, delta[T - 1][i], T - 1, i, psi[T - 1][i]);
}

void baumWelchProcedure(const double alpha[][N], const double beta[][N], double gamma[][N], double xi[][N][N])
{
	int i, j, k, t;
	double numer, denom;

	// Initialization
	for (t = 0; t < T - 1; t++)
	{
		denom = 0.0;
		for (i = 0; i < N; i++)
		{
			gamma[t][i] = 0.0;

			for (j = 0; j < N; j++)
			{
				xi[t][i][j] = alpha[t][i] * A[i][j] * B[j][O[t + 1]] * beta[t + 1][j];
				denom += xi[t][i][j];
			}
		}

		for (i = 0; i < N; i++)
		{
			for (j = 0; j < N; j++)
			{
				if (denom)
					xi[t][i][j] /= denom;
				gamma[t][i] += xi[t][i][j];
			}
		}
	}

	// Last gamma
	denom = 0.0;
	for (i = 0; i < N; i++)
	{
		gamma[T - 1][i] = alpha[T - 1][i];
		denom += gamma[T - 1][i];
	}
	for (i = 0; i < N; i++)
		if (denom)
			gamma[T - 1][i] /= denom;

	// Induction
	// P matrix
	for (i = 0; i < N; i++)
		P[i] = gamma[0][i];

	// A matrix
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			numer = denom = 0.0;
			for (t = 0; t < T - 1; t++)
			{
				numer += xi[t][i][j];
				denom += gamma[t][i];
			}

			if (denom)
				A[i][j] = numer / denom;
		}
	}

	// B matrix
	for (j = 0; j < N; j++)
	{
		for (k = 0; k < M; k++)
		{
			numer = denom = 0.0;
			for (t = 0; t < T; t++)
			{
				if (O[t] == k)
					numer += gamma[t][j];
				denom += gamma[t][j];
			}

			if (denom)
				B[j][k] = numer / denom;
		}
	}
}

int _tmain(int argc, _TCHAR *argv[])
{
	double alpha[T][N], beta[T][N], delta[T][N],
		gamma[T][N], xi[T - 1][N][N],
		probability, p_star;
	int psi[T][N], q_star[T];

	readAMatrix();
	readBMatrix();
	readPMatrix();
	readOMatrix();
	// makeUnbiasedMatrix();

	printMatrix();
	probability = forwardProcedure(alpha);
	backwardProcedure(beta);
	printf("\n\nP(O | y) = %lf\n\n", probability);

	// p_star = viterbiProcedure(A, B, P, O, delta, psi, q_star);
	// printf("\nMost likely state sequence:\n");
	// for (int t = 0; t < T; t++)
	// 	printf("\tq*[%d]\t= S%d\n", t + 1, q_star[t] + 1);
	// printf("\nP* = %lf\n", p_star);

	for (int i = 0; i < 10; i++)
	{
		baumWelchProcedure(alpha, beta, gamma, xi);
		printMatrix();
		probability = forwardProcedure(alpha);
		backwardProcedure(beta);
		printf("\n\nP(O | y) = %lf\n\n", probability);
	}

	finalMatrix();
	return 0;
}
