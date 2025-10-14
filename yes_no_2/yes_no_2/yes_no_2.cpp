// yes_no_2.cpp : Defines the entry point for the console application.
#include "stdafx.h"
using namespace std;

// global variables
#define lineSize 50
#define sample 48000
#define range 100
#define noise 20000
#define yes 40
short int waveIn[16025 * 3];

#pragma comment(lib, "winmm.lib")
void PlayRecord();
void writedataTofile(LPSTR lpData, DWORD dwBufferLength);
void StartRecord();

int _tmain(int argc, _TCHAR *argv[])
{
	StartRecord();
	// used volt instead of energy
	int volt[sample / range], zcr[sample / range];
	int start, end, i, j, k, count1, count2;

	// calculating volt array
	k = 0;
	for (i = 0; i < sample; i++)
	{
		if (i % range == 0)
		{
			volt[i / range] = k;
			k = 0;
		}
		k += abs(waveIn[i]);
	}

	// excluding frames from start and end
	k = 3;
	start = end = -1;

	// determining marker values (excluding the noise part)
	for (i = k; i < sample / range - k; i++)
	{
		if (start == -1 && volt[i] > noise)
			start = i;
		else if (start != -1 && end == -1 && volt[i] < noise)
			end = i;

		// calculating ZCR values
		if (start != -1 && end == -1)
		{
			k = 0;
			for (j = i * range; j < (i + 1) * range; j++)
			{
				if (waveIn[j] * waveIn[j + 1] < 0)
					k++;
			}
			zcr[i] = k;
		}
	}
	// printf("\n\nstart = %d\t\tend = %d", start, end);

	// comparing ZCR values of first half and second half of useful recording
	count1 = count2 = 0;
	for (i = start; i < end; i++)
	{
		if (i <= (start + end) / 2 && zcr[i] > yes)
			count1++;
		else if (i > (start + end) / 2 && zcr[i] > yes)
			count2++;
	}
	printf("\ncount1 = %d\t\tcount2 = %d", count1, count2);

	// determine YES/NO based on ZCR value counts
	if (!count1 && !count2)
		printf("\nNO\n");
	else if (count1 < count2)
		printf("\nYES\n");
	return 0;
}

void StartRecord()
{
	const int NUMPTS = 16025 * 3; // 3 seconds
	int sampleRate = 16025;
	// 'short int' is a 16-bit type; I request 16-bit samples below
	// for 8-bit capture, you'd use 'unsigned char' or 'BYTE' 8-bit     types
	HWAVEIN hWaveIn;
	MMRESULT result;
	WAVEFORMATEX pFormat;
	pFormat.wFormatTag = WAVE_FORMAT_PCM;	  // simple, uncompressed format
	pFormat.nChannels = 1;					  //  1=mono, 2=stereo
	pFormat.nSamplesPerSec = sampleRate;	  // 8.0 kHz, 11.025 kHz, 22.05 kHz, and 44.1 kHz
	pFormat.nAvgBytesPerSec = sampleRate * 2; // =  nSamplesPerSec � nBlockAlign
	pFormat.nBlockAlign = 2;				  // = (nChannels � wBitsPerSample) / 8
	pFormat.wBitsPerSample = 16;			  //  16 for high quality, 8 for telephone-grade
	pFormat.cbSize = 0;
	// Specify recording parameters
	result = waveInOpen(&hWaveIn, WAVE_MAPPER, &pFormat, 0L, 0L, WAVE_FORMAT_DIRECT);
	WAVEHDR WaveInHdr;
	// Set up and prepare header for input
	WaveInHdr.lpData = (LPSTR)waveIn;
	WaveInHdr.dwBufferLength = NUMPTS * 2;
	WaveInHdr.dwBytesRecorded = 0;
	WaveInHdr.dwUser = 0L;
	WaveInHdr.dwFlags = 0L;
	WaveInHdr.dwLoops = 0L;
	waveInPrepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));
	// Insert a wave input buffer
	result = waveInAddBuffer(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));
	// Commence sampling input
	result = waveInStart(hWaveIn);
	cout << "recording for 3 seconds..." << endl;
	Sleep(3 * 1000);
	// Wait until finished recording
	waveInClose(hWaveIn);
	// PlayRecord();
}

void PlayRecord()
{
	const int NUMPTS = 16025 * 3; // 3 seconds
	int sampleRate = 16025;
	// 'short int' is a 16-bit type; I request 16-bit samples below
	// for 8-bit capture, you'd    use 'unsigned char' or 'BYTE' 8-bit types
	HWAVEIN hWaveIn;
	WAVEFORMATEX pFormat;
	pFormat.wFormatTag = WAVE_FORMAT_PCM;	  // simple, uncompressed format
	pFormat.nChannels = 1;					  //  1=mono, 2=stereo
	pFormat.nSamplesPerSec = sampleRate;	  // 44100
	pFormat.nAvgBytesPerSec = sampleRate * 2; // = nSamplesPerSec * n.Channels * wBitsPerSample/8
	pFormat.nBlockAlign = 2;				  // = n.Channels * wBitsPerSample/8
	pFormat.wBitsPerSample = 16;			  //  16 for high quality, 8 for telephone-grade
	pFormat.cbSize = 0;
	// Specify recording parameters
	waveInOpen(&hWaveIn, WAVE_MAPPER, &pFormat, 0L, 0L, WAVE_FORMAT_DIRECT);
	WAVEHDR WaveInHdr;
	// Set up and prepare header for input
	WaveInHdr.lpData = (LPSTR)waveIn;
	WaveInHdr.dwBufferLength = NUMPTS * 2;
	WaveInHdr.dwBytesRecorded = 0;
	WaveInHdr.dwUser = 0L;
	WaveInHdr.dwFlags = 0L;
	WaveInHdr.dwLoops = 0L;
	waveInPrepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));
	HWAVEOUT hWaveOut;
	cout << "playing..." << endl;
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &pFormat, 0, 0, WAVE_FORMAT_DIRECT);
	waveOutWrite(hWaveOut, &WaveInHdr, sizeof(WaveInHdr)); // Playing the data
	Sleep(3 * 1000);									   // Sleep for as long as there was recorded
	waveInClose(hWaveIn);
	waveOutClose(hWaveOut);
}