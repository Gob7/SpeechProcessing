// yesVno.cpp : Defines the entry point for the console application.

#include "stdafx.h"
// Assumptions
#define lineSize 50
#define sample 45000
#define range 100
#define noise 20000
#define yes 40

int _tmain(int argc, _TCHAR* argv[])
{
	char line[lineSize];
	FILE *f;
	int amp[sample], volt[sample/range], zcr[sample/range];
	int size, start, end, i, j, k, count1, count2;
	
	if (fopen_s(&f, "C:\\Users\\panda\\Documents\\M Tech\\Sem 1\\Speech Processing\\Record Yes No\\yes.txt", "r")!=0)
	{
		printf("File not found!\n");
		return 0;
	}
	printf("File found.\n");
	
	// to ignore metadata
	// size = -5
	size = 0;
	while (fgets(line, sizeof(line), f))
	{
		if (size>=0)
		{
			amp[size] = (int)strtol(line, NULL, 10);
			// printf("%d\t", amp[size]);
		}
		// printf(line);
		size++;
		if (size == sample)
		{
			printf("Sample value exceeded.\n");
			return 0;
		}
		/*
		if (size == INT_MIN)
		{
			printf("Size value exceeded.\n");
			return 0;
		}
		*/
	}
	printf("size = %d\n", size);

	k = 0;
	for (i = 0; i < size; i++)
	{
		if (i % range == 0)
		{
			volt[i / range] = k;
			k = 0;
		}
		k += abs(amp[i]);
	}

	start = end = -1;
	for (i=0; i<size/range; i++)
	{
		// printf("%d\t->\t%d\n", i, volt[i]);
		if (start == -1 && volt[i] > noise)
			start = i;
		else if (start != -1 && end == -1 && volt[i] < noise)
			end = i;

		if (start != -1 && end == -1)
		{
			k = 0;
			for (j = i*range; j < (i+1)*range; j++)
			{
				if (amp[j]*amp[j+1] < 0)
					k++;
			}
			zcr[i] = k;
			// printf("%d\t->\t%d\n", i, k);
		}
	}
	// printf("\n\nstart = %d\t\tend = %d", start, end);

	count1 = count2 = 0;
	for (i=start; i<end; i++)
	{
		if (i <= (start+end)/2 && zcr[i] > yes)
			count1++;
		else if (i > (start+end)/2 && zcr[i] > yes)
			count2++;
	}
	printf("\ncount1 = %d\t\tcount2 = %d", count1, count2);

	if (!count1 && !count2)
		printf("\nNO\n");
	else if (count1 < count2)
		printf("\nYES\n");
	return 0;
}

