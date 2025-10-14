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

/////////////////////////////////////////////////////////////////////////////

Code Explanation (yesVno.cpp)
	This program reads a speech signal (from a .txt file containing amplitude samples of a recorded word).
	And classifies whether the spoken word is “YES” or “NO” based on Zero Crossing Rate (ZCR) analysis.

Constants Defined
	lineSize = buffer size for reading lines.
	sample = max number of audio samples.
	range = frame size (100 samples per frame).
	noise = threshold to detect voice activity.
	yes = ZCR threshold to identify "YES".

File Reading
	Opens yes.txt (speech samples).
	Reads amplitude values into amp[].
	Stops if exceeds max samples (45000).

Frame Energy Calculation
	Splits samples into frames (range = 100).
	Computes volt[] = sum of absolute amplitudes in each frame.

Voice Activity Detection
	Detects start (where speech begins) when energy > noise.
	Detects end (where speech ends) when energy falls below noise.

Zero Crossing Rate (ZCR)
	For frames between start and end, counts sign changes (amp[j]*amp[j+1] < 0).
	Stores in zcr[].

Decision Logic
	Splits speech into two halves.
	count1 = frames with ZCR > threshold in first half.
	count2 = frames with ZCR > threshold in second half.
	If count1 < count2 → classify as YES.
	Else → classify as NO.

Instructions to Execute
	Save the code as yesVno.cpp.
	Place your input file yes.txt in the path:
	C:\Users\panda\Documents\M Tech\Sem 1\Speech Processing\yesVnoRecord\
	(or update the path in code).

	Open Visual Studio 2010 (or later).
	Create a Win32 Console Application, add yesVno.cpp to the project.
	Build and run.

	Output will be:
	"YES" → if speech is classified as "YES".
	"NO" → otherwise.

/////////////////////////////////////////////////////////////////////////////