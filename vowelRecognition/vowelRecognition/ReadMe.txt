========================================================================
    CONSOLE APPLICATION : vowelRecognition Project Overview
========================================================================

AppWizard has created this vowelRecognition application for you.

This file contains a summary of what you will find in each of the files that
make up your vowelRecognition application.


vowelRecognition.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

vowelRecognition.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

vowelRecognition.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named vowelRecognition.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

1. OVERVIEW
	This program performs vowel recognition using Linear Predictive Coding (LPC) and Cepstral Analysis.
	
	It has two modes:
		Training Mode (TEST = false)
			Processes 20 utterances per vowel.
			Computes average cepstral coefficients for each vowel.
			Saves reference templates for future testing.

		Testing Mode (TEST = true)
			Processes 10 test utterances per vowel.
			Compares each utterance against reference templates using Tokhura’s Distance.
			Outputs the recognized vowel and calculates accuracy.



2. CONSTANTS DEFINED
	Constant		Value			Purpose
	PI				3.14159			Mathematical constant for windowing.
	X				42000			Maximum number of samples per file.
	N				320				Frame size (samples per frame).
	OVER			160				50% frame overlap (N/2).
	S				100				Buffer size for file paths and strings.
	F_TRAIN			20				Number of training utterances per vowel.
	F_TEST			10				Number of testing utterances per vowel.
	P				12				LPC order (12 coefficients).
	W				5				Number of frames used per utterance.
	V				5				Number of vowels (a, e, i, o, u).
	VOWELS[]		{a,e,i,o,u}		Vowel list for file naming.
	WEIGHTS[]						Weights used in Tokhura’s distance calculation.
					{1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0}



3. PROGRAM FLOW

(I) Training Phase (TEST = false)
	Goal: Create reference templates for each vowel.

	Steps:
	a)Input Files
		Each vowel has 20 training files:
		254101015_a_1.txt, 254101015_a_2.txt, ..., 254101015_a_20.txt

	b)File Reading (takeInput)
		Reads amplitude samples into amp[].
		Stops if size exceeds X.

	c)Noise Removal (cutNoise)
		Divides signal into frames of size N.
		Computes energy for each frame (volt[]).
		Finds START and END positions to trim silence/noise.

	d)dcShift() → Remove DC bias.
	e)normalize() → Normalize signal amplitude.

	f)Feature Extraction
		Auto-Correlation (riCompute) → Computes r[i] for each frame.
		LPC Coefficients (aiCompute) → Uses Levinson-Durbin algorithm to compute a[i].
		Cepstral Coefficients (ciCompute) → Converts LPC coefficients to cepstral coefficients c[i].
		Raised Sine Window (ciRaisedSine) → Smooths cepstral features into c_sin[i].

	g)Aggregate Features (addCI and updateCI)
		Averages cepstral coefficients across 20 utterances to form the reference template.

	h)Save Reference Template (giveOutput)
		Creates one .txt file per vowel containing the averaged coefficients.


(II) Testing Phase (TEST = true)
	Goal: Recognize unknown vowels using reference templates.

	a)Load Reference Templates (readData)
		Reads the five reference template files generated during training.

	b)Process Test Files
		Each vowel has 10 test files:
		254101015_a_21.txt, 254101015_a_22.txt, ..., 254101015_a_30.txt

	c)Feature Extraction (same steps as training)
		cutNoise() → Trim silence.
		riCompute(), aiCompute(), ciCompute(), ciRaisedSine() → Compute cepstral coefficients.

	d)Classification (tokhuraDistance)
		Compares test cepstral coefficients with each vowel reference using Tokhura's Distance:
			distance = Σ(weights[k] * (c_test - c_ref)^2)
		Chooses the vowel with minimum distance.

	e)Output Results
		Prints predicted vowel and calculates accuracy per vowel:
		Accuracy = Correct Predictions / F_TEST



4. KEY FUNCTIONS
	Function				Purpose
	takeInput()				Reads speech samples from input .txt file.
	cutNoise()				Detects start and end of speech by energy thresholding.
	dcShift()				Remove DC bias.
	e)normalize()			Normalize signal amplitude.
	riCompute()				Computes auto-correlation values for each frame.
	aiCompute()				Computes LPC coefficients using Levinson-Durbin algorithm.
	ciCompute()				Converts LPC coefficients to cepstral coefficients.
	ciRaisedSine()			Applies raised sine window to cepstral coefficients.
	addCI() / updateCI()	Averages cepstral coefficients over multiple utterances.
	tokhuraDistance()		Compares test data to reference templates using weighted distance.
	giveOutput()			Saves reference template to a file.
	readData()				Loads reference templates for testing.



5. INPUT/OUTPUT FILE STRUCTURE
		Training Input Files
		254101015_a_1.txt ... 254101015_a_20.txt
		254101015_e_1.txt ... 254101015_e_20.txt
		254101015_i_1.txt ... 254101015_i_20.txt
		254101015_o_1.txt ... 254101015_o_20.txt
		254101015_u_1.txt ... 254101015_u_20.txt

		Generated Reference Files
		254101015_a.txt
		254101015_e.txt
		254101015_i.txt
		254101015_o.txt
		254101015_u.txt

		Testing Input Files
		254101015_a_21.txt ... 254101015_a_30.txt
		254101015_e_21.txt ... 254101015_e_30.txt
		254101015_i_21.txt ... 254101015_i_30.txt
		254101015_o_21.txt ... 254101015_o_30.txt
		254101015_u_21.txt ... 254101015_u_30.txt



6. PROGRAM EXECUTION
	Training Mode
		Set:
		bool TEST = false;
		Run the program → generates reference files for each vowel.

	Testing Mode
		Set:
		bool TEST = true;
		Run the program → loads reference templates and classifies each test file.



7. SAMPLE CONSOLE OUTPUT
	For 'a':        (21)a   (22)a   (23)a   (24)a   (25)a   (26)a   (27)e   (28)a   (29)a   (30)a
	Accuracy: 0.90

	For 'e':        (21)e   (22)e   (23)i   (24)e   (25)e   (26)e   (27)e   (28)o   (29)e   (30)e
	Accuracy: 0.80



8. SUMMARY OF KEY ARRAYS
	Array		Size		Purpose
	amp[]		42000		Raw speech signal samples.
	volt[]		X/N			Energy per frame for noise detection.
	r[][]		[W][P+1]	Auto-correlation values for each frame.
	a[][]		[W][P+1]	LPC coefficients.
	c[][]		[W][P+1]	Cepstral coefficients.
	c_sin[][]	[W][P+1]	Raised sine windowed cepstral coefficients.
	c_ref[][][]	[V][W][P]	Stored reference templates for each vowel.



9. Classification Logic
	Compute Tokhura's Distance between test cepstral vector and each vowel reference:
	distance = Σ (WEIGHTS[k] * (c_test[k] - c_ref[k])²)

	Prediction Rule:
	Recognized vowel = vowel with minimum distance



10. Summary
	This program provides a complete pipeline for vowel recognition:
	Training Mode: Extracts and averages cepstral features.
	Testing Mode: Uses Tokhura’s distance to classify test utterances.
	Implements LPC-based speech feature extraction and template-based matching for recognition.


//////////////////////////////////////////////////////////////////////////////////////////////////////////////