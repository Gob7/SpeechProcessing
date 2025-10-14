========================================================================
    CONSOLE APPLICATION : yes_no_2 Project Overview
========================================================================

AppWizard has created this yes_no_2 application for you.

This file contains a summary of what you will find in each of the files that
make up your yes_no_2 application.


yes_no_2.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

yes_no_2.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

yes_no_2.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named yes_no_2.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////

Code Explanation (record_wave.cpp)
	This program records a 3-second audio sample using the microphone, processes the signal.
	And classifies the spoken word as “YES” or “NO” based on Zero Crossing Rate (ZCR).

Global Setup
	sample = 48000 → total samples (3 sec at 16 kHz).
	range = 100 → frame size.
	noise = 20000 → threshold to detect voice.
	yes = 40 → ZCR threshold for “YES”.
	waveIn[] → buffer storing recorded waveform samples.

Recording
	StartRecord() → records audio for 3 seconds into waveIn.
	PlayRecord() (declared but unused here) could play back recorded voice.
	writedataTofile() can store amplitudes into a file (if used).

Feature Extraction
	volt[] → sum of absolute amplitudes per frame (proxy for energy).
	Detects start (speech onset) when volt > noise.
	Detects end (speech offset) when volt < noise.

Zero Crossing Rate (ZCR)
	Between start and end, counts sign changes (waveIn[j] * waveIn[j+1] < 0).
	Stores per-frame ZCR in zcr[].

Classification Logic
	Splits active speech into two halves.
	count1 = frames with ZCR > threshold in first half.
	count2 = frames with ZCR > threshold in second half.
		If count1 < count2 → classify as YES.
		Otherwise → classify as NO.

/////////////////////////////////////////////////////////////////////////////

Instructions to Execute:
	Save code as yes_no_2.cpp.
	Open Visual Studio 2010 (or later).
	Create a Win32 Console Application and add this file.
	Ensure winmm.lib (Windows Multimedia library) is linked (already handled by #pragma comment(lib, "winmm.lib")).

	Build and run.
	Speak clearly into the microphone when recording starts.

	Output will be:
	"YES" if detected word is “YES”.
	"NO" otherwise.

/////////////////////////////////////////////////////////////////////////////