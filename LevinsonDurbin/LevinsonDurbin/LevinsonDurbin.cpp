// LevinsonDurbin.cpp : Defines the entry point for the console application.

// Initialization
#include "stdafx.h"
#define lineSize 100
#define sample 42000
#define range 100
#define noise 20000
#define p 12

int _tmain(int argc, _TCHAR *argv[])
{
	// Variables
	FILE *fin, *fout;
	char line[lineSize], input[lineSize], output[lineSize];
	float amp[sample], volt[sample / range], E, k, sum;
	int size, start, end, i, j;

	for (int x = 1; x <= 30; x++)
	{
		printf("\n\n\n%d\t", x);
		sprintf(input, "C:\\Users\\panda\\Documents\\M Tech\\Sem 1\\Speech Processing\\Record Vowels\\254101015_u_%d.txt", x);

		// Input
		float r[p + 1] = {0}, a[p + 1] = {0}, a_prev[p + 1] = {0};

		// Open input file
		if (fopen_s(&fin, input, "r") != 0)
		{
			printf("File not found!\n");
			return 0;
		}
		printf("File found.\n");

		// Compute file size
		size = 0;
		while (fgets(line, sizeof(line), fin))
		{
			amp[size] = atof(line);
			// printf("%f\n", amp[size]);
			size++;

			if (size == sample)
			{
				printf("Sample value exceeded.\n");
				return 0;
			}
		}
		printf("size = %d\n", size);
		fclose(fin);

		// Compute volt (enery substitute) array
		k = 0;
		for (i = 0; i < size; i++)
		{
			if (i % range == 0)
			{
				volt[i / range] = k;
				// printf("%f\n", k);
				k = 0;
			}
			k += abs(amp[i]);
		}

		// Find start and end markers
		start = end = -1;
		i = 0;
		while (start == -1 && i < size / range)
		{
			if (volt[i] > noise)
				start = i;
			i++;
		}
		i = size / range;
		while (end == -1 && i > 0)
		{
			if (volt[i] > noise)
				end = i;
			i--;
		}
		printf("start = %d\t\tend = %d\n\n", start, end);

		// Autocorelation

		// Calculating ri's
		for (i = 0; i <= p; i++)
		{
			for (j = start; j < end - i; j++) // 0, size --- start, end
				r[i] += amp[j] * amp[j + i];
			// printf("\n%f", r[i]);
		}

		// Calculating ai's (Levinson-Durbin Algorithm)
		E = r[0];
		for (i = 1; i <= p; i++)
		{
			sum = 0.0f;

			for (j = 1; j < i; j++)
				sum += a_prev[j] * r[i - j];

			// Reflection coefficient
			k = (r[i] - sum) / E;

			// Update LPC coefficients
			a[i] = k;
			for (j = 1; j < i; j++)
				a[j] = a_prev[j] - k * a_prev[i - j];

			// Update prediction error
			E *= (1.0f - k * k);

			// Copy current a[] to a_prev[] for next iteration
			for (j = 1; j <= i; j++)
				a_prev[j] = a[j];
		}

		// Output

		// Derive output file name
		strcpy(output, input);
		char *dot = strrchr(output, '.');
		if (dot)
			*dot = '\0';
		strcat(output, "_ai.txt");

		// Open output file
		if (fopen_s(&fout, output, "w") != 0)
			return 0;

		// Write data
		for (i = 1; i <= p; i++)
		{
			printf("%.5f    ", a[i]);
			fprintf(fout, "%f\n", a[i]);
		}
		fclose(fout);

	} // endfor
	return 0;
}
