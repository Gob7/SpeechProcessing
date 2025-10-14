========================================================================
    CONSOLE APPLICATION : LevinsonDurbin Project Overview
========================================================================

AppWizard has created this LevinsonDurbin application for you.

This file contains a summary of what you will find in each of the files that
make up your LevinsonDurbin application.


LevinsonDurbin.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

LevinsonDurbin.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

LevinsonDurbin.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named LevinsonDurbin.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////

Overview:
	This program processes speech recordings of vowel sounds and extracts Linear Predictive Coding (LPC) coefficients (a1 to a12).
	These coefficients are useful in speech recognition, feature extraction, and pattern classification.

	The program:
		Reads raw speech sample files (.txt format).
		Detects the voice region of the signal.
		Calculates autocorrelation coefficients (ri).
		Uses Levinson-Durbin Algorithm to compute LPC coefficients.
		Saves LPC coefficients into output files.

Constants Defined:
	Constant		Value		Purpose
	lineSize		100			Buffer size for reading lines.
	sample			42000		Maximum audio samples allowed.
	range			100			Frame size (100 samples per frame).
	noise			20000		Threshold for detecting start and end.
	p				12			Number of LPC coefficients to compute.

Program Flow:
	Step 1 – Processing Multiple Files
	step 2 – File Reading
	Step 3 – Energy Calculation (volt[])
	Step 4 – Voice Activity Detection
	Step 5 – Autocorrelation Computation
	Step 6 – Levinson-Durbin Algorithm
	Step 7 – Save Output


Instructions to Run:
	Save this code as vowelRecognition.cpp.
	Ensure your 30*5 input files are stored.
	Open Visual Studio 2010 (or later).
	Create a Win32 Console Application.
	Add vowelRecognition.cpp to the project.
	Build and Run.

Summary of Key Arrays:
	Array		Purpose
	amp[]		Stores raw speech amplitude samples.
	volt[]		Holds energy per frame for voice detection.
	r[]			Autocorrelation coefficients (r0 to r12).
	a[]			LPC coefficients to be calculated (a1-a12).
	a_prev[]	Previous iteration's LPC coefficients.

Key Features:
	Automatic processing of multiple speech files.
	Start and end detection using energy threshold.
	Autocorrelation-based LPC feature extraction.
	Saves results in separate files for further analysis.

/////////////////////////////////////////////////////////////////////////////