// vowelRecognition.cpp : Defines the entry point for the console application.

// Initialization
#include "stdafx.h"
#define PI 3.14159265358979323846

#define X 42000	   // maximum number of samples
#define N 320	   // number of samples per frame
#define S 100	   // line size
#define F_TRAIN 20 // number of utterance of each vowel to train
#define F_TEST 10  // number of utterance of each vowel to test
#define P 12	   // LPC prediction order
#define W 5		   // number of frames
#define V 5		   // number of vowels

int OVER = N / 2; // 50% samples overlap
char VOWELS[V] = {'a', 'e', 'i', 'o', 'u'};
float WEIGHTS[P] = {1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0};

int takeInput(char *input, float *amp)
{
	FILE *fin;
	char line[S];
	int SIZE = 0;

	// Open input file
	if (fopen_s(&fin, input, "r") != 0)
		return 0;

	// Read data
	while (fgets(line, sizeof(line), fin))
	{
		amp[SIZE] = atof(line);
		// printf("%f\n", amp[SIZE]);
		SIZE++;
		if (SIZE == X)
		{
			printf("Size exceeded.\n");
			return 0;
		}
	}
	// printf("SIZE = %d\n", SIZE);

	fclose(fin);
	return SIZE;
}

void cutNoise(float *amp, int SIZE, int *start, int *end)
{
	int i, j, NOISE = 100000; // threshold value
	float volt[X / N], k;

	// Compute volt (energy substitute) array
	for (i = 0; i < SIZE / N; i++)
	{
		k = 0;
		for (j = 0; j < N; j++)
			k += abs(amp[i * N + j]);
		volt[i] = k;
		// printf("%d -> %f\n", i, volt[i]);
	}

	// Find start & end markers
	*start = -1;
	*end = -1;
	i = 0;
	while (*start == -1 && i < SIZE / N)
	{
		if (volt[i] > NOISE)
			*start = i;
		i++;
	}
	i = SIZE / N;
	while (*end == -1 && i > 0)
	{
		i--;
		if (volt[i] > NOISE)
			*end = i;
	}
	// printf("start = %d\t\tend = %d\n", *start, *end);
}

void dcShift()
{
}

void normalize()
{
}

void hammingWindow(float *amp, float *amp_ham, int INIT)
{
	int i, m;
	float w;

	for (i = INIT; i < INIT + N; i++)
	{
		m = i - INIT;
		w = 0.54f - 0.46f * cosf(2.0f * PI * m / (N - 1));
		amp_ham[m] = amp[m] * w;
	}
}

void riCompute(float *amp, float r[][P + 1], int start, int end, int HAM)
{
	// Auto Corelation Values
	int i, j, k;
	float amp_ham[N];

	// Taking steady (middle) part
	start += (end - start) / 2;
	start *= N;
	// printf("new start = %d\n\n", start);

	if (HAM)
		hammingWindow(amp, amp_ham, start);

	for (k = 0; k < W; k++)
	{
		for (i = 0; i <= P; i++)
		{
			for (j = start; j < start + N - i; j++)
			{
				if (HAM)
					r[k][i] += amp_ham[j] * amp_ham[j + i];
				else
					r[k][i] += amp[j] * amp[j + i];
			}
		}
		start += OVER;
	}
}

void aiCompute(float r[][P + 1], float a[][P + 1])
{
	// LPC coefficients (Levinson-Durbin Algorithm)
	int i, j, n;
	float E, K, SUM, a_pre[W][P + 1] = {0};

	for (n = 0; n < W; n++)
	{
		E = r[n][0];
		for (i = 1; i <= P; i++)
		{
			SUM = 0.0f;
			for (j = 1; j < i; j++)
				SUM += a_pre[n][j] * r[n][i - j];

			K = (r[n][i] - SUM) / E; // reflection coefficient
			a[n][i] = K;			 // update LPC coefficients
			for (j = 1; j < i; j++)
				a[n][j] = a_pre[n][j] - K * a_pre[n][i - j];

			E *= 1.0f - K * K;		 // update prediction error
			for (j = 1; j <= i; j++) // update a_pre[]
				a_pre[n][j] = a[n][j];
		}
	}
}

void ciCompute(float r[][P + 1], float a[][P + 1], float c[][P + 1])
{
	// Cepstral coefficients
	int i, m, k;
	float SUM;

	for (i = 0; i < W; i++)
	{
		c[i][0] = log(r[i][0]);
		for (m = 1; m <= P; m++)
		{
			SUM = 0.0f;
			for (k = 1; k < m; k++)
				SUM += ((float)k / m) * c[i][k] * a[i][m - k];

			c[i][m] = a[i][m] + SUM;
		}
	}
}

void ciRaisedSine(float c[][P + 1], float c_sin[][P + 1])
{
	for (int k = 0; k < W; k++)
		for (int m = 1; m <= P; m++)
			c_sin[k][m] = c[k][m] * (1.0f + (P / 2.0f) * sinf(PI * m / P));
}

int giveOutputAll(char *input, float arr[][P + 1], int flag)
{
	char output[S];
	int i, j, k = 1;
	FILE *fout;

	// Derive output file name
	strcpy(output, input);
	char *dot = strrchr(output, '.');
	if (dot)
		*dot = '\0';

	if (flag == 0)
		strcat(output, "_ri.txt");
	else if (flag == 1)
		strcat(output, "_ri_ham.txt");
	else if (flag == 2)
		strcat(output, "_ai.txt");
	else if (flag == 3)
		strcat(output, "_ai_ham.txt");
	else if (flag == 4)
		strcat(output, "_ci.txt");
	else if (flag == 5)
		strcat(output, "_ci_ham.txt");
	else if (flag == 6)
		strcat(output, "_ci_sin.txt");

	if (flag < 2)
		k = 0;

	// Open output file
	if (fopen_s(&fout, output, "w") != 0)
		return 0;

	// Write data
	for (i = 0; i < W; i++)
	{
		for (j = k; j <= P; j++)
		{
			// printf("%.5f    ", arr[i][j]);
			fprintf(fout, "%f\n", arr[i][j]);
		}
		// printf("\n");
		fprintf(fout, "\n");
	}

	fclose(fout);
	return 1;
}

void addCI(float arr[][P + 1], float c_fin[][P])
{
	for (int i = 0; i < W; i++)
		for (int j = 1; j <= P; j++)
			c_fin[i][j - 1] += arr[i][j];
}

void updateCI(float c_fin[][P])
{
	for (int i = 0; i < W; i++)
		for (int j = 0; j < P; j++)
			c_fin[i][j] /= (float)F_TRAIN;
}

int giveOutput(char *input, float arr[][P])
{
	char output[S];
	int i, j, k = 1;
	FILE *fout;

	// Derive output file name
	strcpy(output, input);
	char *pos = strrchr(output, '_'); // find last '_'
	if (pos)
		*pos = '\0'; // terminate before last '_'
	strcat(output, ".txt");

	// Open output file
	if (fopen_s(&fout, output, "w") != 0)
		return 0;

	// Write data
	for (i = 0; i < W; i++)
	{
		for (j = 0; j < P; j++)
		{
			// printf("%.5f    ", arr[i][j]);
			fprintf(fout, "%f\n", arr[i][j]);
		}
		// printf("\n");
		fprintf(fout, "....................\n");
	}

	fclose(fout);
	return 1;
}

int readData(float c_ref[][W][P])
{
	int i, j, k;
	char input[S], line[S];
	FILE *fin;

	for (i = 0; i < V; i++)
	{
		sprintf(input, "C:\\Users\\panda\\Documents\\M Tech\\Sem 1\\Speech Processing\\Record Vowels\\254101015_%c.txt", VOWELS[i]);

		// Open input file
		if (fopen_s(&fin, input, "r") != 0)
			return 0;

		// Read data
		j = 0;
		k = 0;
		while (fgets(line, sizeof(line), fin))
		{
			if (k == P)
			{
				k = 0;
				j++;
				// printf("\n");
				continue;
			}

			c_ref[i][j][k] = atof(line);
			// printf("%.5f     ", c_ref[i][j][k]);
			k++;
		}

		// printf("\n");
		fclose(fin);
	}
	return 1;
}

int tokhuraDistance(float c_ref[][W][P], float c[][P + 1])
{
	float distance, dist, d, minDist = FLT_MAX;
	int i, j, k, index;

	for (i = 0; i < V; i++)
	{
		distance = 0.0f;
		for (j = 0; j < W; j++)
		{
			dist = 0.0f;
			for (k = 0; k < P; k++)
			{
				d = c[j][k + 1] - c_ref[i][j][k];
				dist += WEIGHTS[k] * d * d;
			}
			distance += dist;
		}
		distance /= (float)W;

		if (minDist > distance)
		{
			minDist = distance;
			index = i;
		}
	}
	return index;
}

int _tmain(int argc, _TCHAR *argv[])
{
	char input[S];
	float amp[X];
	int i, j, k, acc, SIZE, START, END;
	bool TEST = true;
	float c_fin[W][P] = {0}, c_ref[V][W][P] = {0};

	// Training (20 files)
	i = TEST ? V : 0;
	for (; i < V; i++)
	{
		for (j = 1; j <= F_TRAIN; j++)
		{
			// printf("\n\n\n%d\t", j);
			sprintf(input, "C:\\Users\\panda\\Documents\\M Tech\\Sem 1\\Speech Processing\\Record Vowels\\254101015_%c_%d.txt", VOWELS[i], j);
			// sprintf(input, "C:\\Users\\panda\\Documents\\M Tech\\Sem 1\\Speech Processing\\Record Vowels\\test.txt");

			float
				r[W][P + 1] = {0},
						 r_ham[W][P + 1] = {0},
						 a[W][P + 1] = {0},
						 a_ham[W][P + 1] = {0},
						 c[W][P + 1] = {0},
						 c_ham[W][P + 1] = {0},
						 c_sin[W][P + 1] = {0};

			SIZE = takeInput(input, amp); // SIZE: total number of samples read from file
			if (!SIZE)
				return 0;

			cutNoise(amp, SIZE, &START, &END); // START, END: markers for vowel start and end (cut the silence part)
			dcShift();
			normalize();

			if (END - START < (W - 1) * (N - OVER) / N)
			{
				printf("Not enough samples.\n");
				return 0;
			}

			riCompute(amp, r, START, END, 0); // ri's
			// riCompute(amp, r_ham, START, END, 1);	// ri's + hamming window

			aiCompute(r, a); // ai's
			// aiCompute(r_ham, a_ham);					// ai's + hamming window

			ciCompute(r, a, c); // ci's
			// ciCompute(r_ham, a_ham, c_ham);			// ci's + hamming window
			ciRaisedSine(c, c_sin); // ci's + raised sine window

			addCI(c_sin, c_fin);

			/*
			k = giveOutputAll(input, r, 0);
			if (!k)
				return 0;

			k = giveOutputAll(input, r_ham, 1);
			if (!k)
				return 0;

			k = giveOutputAll(input, a, 2);
			if (!k)
				return 0;

			k = giveOutputAll(input, a_ham, 3);
			if (!k)
				return 0;

			k = giveOutputAll(input, c, 4);
			if (!k)
				return 0;

			k = giveOutputAll(input, c_ham, 5);
			if (!k)
				return 0;

			k = giveOutputAll(input, c_sin, 6);
			if (!k)
				return 0;
			*/
		}

		updateCI(c_fin);
		k = giveOutput(input, c_fin);
		printf("\n");
		if (!k)
			return 0;
	}

	// Testing (10 files)
	if (TEST)
	{
		i = 0;
		k = readData(c_ref);
		if (!k)
			return 0;
	}
	else
		i = V;

	for (; i < V; i++)
	{
		printf("For '%c':\t\t", VOWELS[i]);
		acc = 0;
		for (j = 1 + F_TRAIN; j <= F_TRAIN + F_TEST; j++)
		{
			printf("(%d)", j);
			sprintf(input, "C:\\Users\\panda\\Documents\\M Tech\\Sem 1\\Speech Processing\\Record Vowels\\254101015_%c_%d.txt", VOWELS[i], j);

			float
				r[W][P + 1] = {0},
						 a[W][P + 1] = {0},
						 c[W][P + 1] = {0},
						 c_sin[W][P + 1] = {0};

			SIZE = takeInput(input, amp);
			if (!SIZE)
				return 0;

			cutNoise(amp, SIZE, &START, &END);
			dcShift();
			normalize();

			riCompute(amp, r, START, END, 0);
			aiCompute(r, a);
			ciCompute(r, a, c);
			ciRaisedSine(c, c_sin);

			k = tokhuraDistance(c_ref, c_sin);
			if (k == i)
				acc++;
			printf("%c\t", VOWELS[k]);
		}
		printf("\nAccuracy: %.2f\n\n", (float)acc / F_TEST);
	}

	return 0;
}