========================================================================
    CONSOLE APPLICATION : yesVno Project Overview
========================================================================

AppWizard has created this yesVno application for you.

This file contains a summary of what you will find in each of the files that
make up your yesVno application.


yesVno.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

yesVno.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

yesVno.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named yesVno.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

1. OVERVIEW
	C++ console program that records or reads a waveform, 
	computes frame energy and zero-crossing rate (ZCR), 
	and classifies a recorded utterance as YES, NO, or UNDETERMINED using ZCR-based heuristics.



2. CONSTANTS & GLOBAL VARIABLES
	Constant	Value			Purpose
	flag 		non-zero		record + playback; 
				zero 			read from file.
	lineSize 	50 				file read buffer.
	second 		3 				recording duration in seconds.
	sample 		16025 			sampling frequency (samples/sec).
	range 		100	 			frame length (samples per frame).
	yesZCR 		40 				ZCR threshold for counting a frame as high-ZCR.
	waveIn[sample * second]		global buffer for recorded/loaded samples.



3. ALGORITHM
	a. Input — either record audio for 3 seconds (Windows waveIn) or read integer samples from yes2.txt.
	b. Frame energy — compute per-frame sum of absolute samples to detect voiced region (start/end).
	c. ZCR computation — for frames inside detected region, count sign changes in each frame.
	d. Decision rule — split detected region into two halves; count frames with ZCR > yesZCR in first half (c1) and second half (c2):
			if both zero → print NO,
			else if c1 < c2 → print YES,
			else → print nothing.



4. KEY FUNCTIONS
	Function				Description
	StartRecord() 			captures audio into waveIn using the Windows multimedia API (3 s).
	PlayRecord()	 		plays contents of waveIn via waveOut.
	takeInput()				reads samples from (yes.txt / no.txt) into waveIn; returns sample count.
	findZeroCrossingRate() 	computes per-frame energy to detect start/end and fills ZCR array for frames between start and end.
	determineYesNo() 		applies the decision logic on ZCR counts and returns 1 (YES), 0 (NO), or -1 (nothing).



5. EXECUTION
	a. Build as a Win32 console application (Visual Studio).
	b. To record + play set #define flag 1 (default in code). Run program; it records 3 seconds, plays back, then processes.
	c. To use file input set #define flag 0, set path to (yes.txt / no.txt), then run.
	d. Output appears on console: SIZE, counts, then YES or NO.

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////