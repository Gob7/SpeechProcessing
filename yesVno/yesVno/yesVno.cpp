// yesVno.cpp : Defines the entry point for the console application.
#include "stdafx.h"
using namespace std;

// global variables
#define flag 1
#define lineSize 50
#define second 3
#define sample 16025
#define range 100
#define yesZCR 40

short int waveIn[sample * second];
#pragma comment(lib, "winmm.lib")

void PlayRecord()
{
	// 'short int' is a 16-bit type; I request 16-bit samples below
	// for 8-bit capture, you'd    use 'unsigned char' or 'BYTE' 8-bit types
	const int NUMPTS = sample * second; // 3 seconds
	int sampleRate = sample;

	// Specify recording parameters
	HWAVEIN hWaveIn;
	WAVEFORMATEX pFormat;
	pFormat.wFormatTag = WAVE_FORMAT_PCM;	  // simple, uncompressed format
	pFormat.nChannels = 1;					  //  1=mono, 2=stereo
	pFormat.nSamplesPerSec = sampleRate;	  // 44100
	pFormat.nAvgBytesPerSec = sampleRate * 2; // = nSamplesPerSec * n.Channels * wBitsPerSample/8
	pFormat.nBlockAlign = 2;				  // = n.Channels * wBitsPerSample/8
	pFormat.wBitsPerSample = 16;			  //  16 for high quality, 8 for telephone-grade
	pFormat.cbSize = 0;

	// Set up and prepare header for input
	waveInOpen(&hWaveIn, WAVE_MAPPER, &pFormat, 0L, 0L, WAVE_FORMAT_DIRECT);

	WAVEHDR WaveInHdr;
	WaveInHdr.lpData = (LPSTR)waveIn;
	WaveInHdr.dwBufferLength = NUMPTS * 2;
	WaveInHdr.dwBytesRecorded = 0;
	WaveInHdr.dwUser = 0L;
	WaveInHdr.dwFlags = 0L;
	WaveInHdr.dwLoops = 0L;

	waveInPrepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));

	HWAVEOUT hWaveOut;

	cout << "Playing..." << endl;
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &pFormat, 0, 0, WAVE_FORMAT_DIRECT);
	waveOutWrite(hWaveOut, &WaveInHdr, sizeof(WaveInHdr)); // Playing the data
	Sleep(3 * 1000);									   // Sleep for as long as there was recorded

	waveInClose(hWaveIn);
	waveOutClose(hWaveOut);
}

void StartRecord()
{
	// 'short int' is a 16-bit type; I request 16-bit samples below
	// for 8-bit capture, you'd use 'unsigned char' or 'BYTE' 8-bit types
	const int NUMPTS = sample * second; // 3 seconds
	int sampleRate = sample;

	// Specify recording parameters
	HWAVEIN hWaveIn;
	MMRESULT result;
	WAVEFORMATEX pFormat;
	pFormat.wFormatTag = WAVE_FORMAT_PCM;	  // simple, uncompressed format
	pFormat.nChannels = 1;					  //  1=mono, 2=stereo
	pFormat.nSamplesPerSec = sampleRate;	  // 8.0 kHz, 11.025 kHz, 22.05 kHz, and 44.1 kHz
	pFormat.nAvgBytesPerSec = sampleRate * 2; // =  nSamplesPerSec * nBlockAlign
	pFormat.nBlockAlign = 2;				  // = (nChannels * wBitsPerSample) / 8
	pFormat.wBitsPerSample = 16;			  //  16 for high quality, 8 for telephone-grade
	pFormat.cbSize = 0;

	// Set up and prepare header for input
	result = waveInOpen(&hWaveIn, WAVE_MAPPER, &pFormat, 0L, 0L, WAVE_FORMAT_DIRECT);

	WAVEHDR WaveInHdr;
	WaveInHdr.lpData = (LPSTR)waveIn;
	WaveInHdr.dwBufferLength = NUMPTS * 2;
	WaveInHdr.dwBytesRecorded = 0;
	WaveInHdr.dwUser = 0L;
	WaveInHdr.dwFlags = 0L;
	WaveInHdr.dwLoops = 0L;

	// Insert a wave input buffer
	waveInPrepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));

	// Commence sampling input
	result = waveInAddBuffer(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));

	// Wait until finished recording
	result = waveInStart(hWaveIn);
	cout << "recording for 3 seconds..." << endl;
	Sleep(3 * 1000);

	waveInClose(hWaveIn);
}

int takeInput()
{
	char line[lineSize];
	FILE *fin;
	int size = 0;

	if (fopen_s(&fin, "C:\\Users\\panda\\Documents\\M Tech\\Sem 1\\Speech Processing\\Record Yes No\\yes2.txt", "r"))
		return 0;

	while (fgets(line, sizeof(line), fin))
	{
		if (size >= 0)
			waveIn[size] = (int)strtol(line, NULL, 10);

		size++;
		if (size == sample * second || size == INT_MIN)
		{
			printf("Sample value exceeded.\n");
			return 0;
		}
	}
	fclose(fin);

	return size;
}

void findZeroCrossingRate(const int SIZE, int *start, int *end, int *zeroCrossingRate)
{
	int voltage[sample * second / range],
		i, j, k,
		NOISE = 50000; // threshold value

	// Compute voltage (energy substitute) array
	for (i = 0; i < SIZE / range; i++)
	{
		k = 0;
		for (j = 0; j < range; j++)
			k += abs(waveIn[i * range + j]);
		voltage[i] = k;
		// printf("%d -> %d\n", i, voltage[i]);
	}

	// Find start & end markers
	*start = -1;
	*end = -1;
	k = 3;
	i = k;
	while (*start == -1 && i < SIZE / range)
	{
		if (voltage[i] > NOISE)
			*start = i;
		i++;
	}
	i = SIZE / range - k;
	while (*end == -1 && i > 0)
	{
		i--;
		if (voltage[i] > NOISE)
			*end = i;
	}

	// calculating ZCR values
	for (i = *start; i < *end; i++)
	{
		k = 0;
		for (j = i * range; j < (i + 1) * range; j++)
		{
			if (waveIn[j] * waveIn[j + 1] < 0)
				k++;
		}
		zeroCrossingRate[i] = k;
		// printf("zcr[%d] = %d\n", i, k);
	}
}

int determineYesNo(const int *start, const int *end, const int *zeroCrossingRate)
{
	int i = *start,
		c1 = 0, c2 = 0,
		mid = (*start + *end) / 2;

	for (; i < mid; i++)
		if (zeroCrossingRate[i] > yesZCR)
			c1++;

	for (; i < *end; i++)
		if (zeroCrossingRate[i] > yesZCR)
			c2++;

	printf("\ncount1 = %d\t\tcount2 = %d", c1, c2);

	if (!c1 && !c2)
		return 0;
	if (c1 < c2)
		return 1;

	return -1;
}

int _tmain(int argc, _TCHAR *argv[])
{
	int zeroCrossingRate[sample * second / range],
		SIZE = sample * second,
		start, end, yes;

	if (flag)
	{
		StartRecord();
		PlayRecord();
	}
	else
	{
		SIZE = takeInput();
		if (!SIZE)
			return 0;
	}

	printf("SIZE = %d\n", SIZE);
	findZeroCrossingRate(SIZE, &start, &end, zeroCrossingRate);
	yes = determineYesNo(&start, &end, zeroCrossingRate);

	if (yes == 1)
		printf("\nYES\n");
	if (yes == 0)
		printf("\nNO\n");
	return 0;
}
