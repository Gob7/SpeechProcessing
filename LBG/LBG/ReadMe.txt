========================================================================
    CONSOLE APPLICATION : LBG Project Overview
========================================================================

AppWizard has created this LBG application for you.

This file contains a summary of what you will find in each of the files that
make up your LBG application.


LBG.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

LBG.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

LBG.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named LBG.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

1. OVERVIEW
	This program implements the Linde–Buzo–Gray (LBG) algorithm, 
		an iterative clustering technique used in Vector Quantization (VQ) for speech signal processing. 
	It generates an optimal codebook from a set of training feature vectors by 
		repeatedly splitting and refining clusters to minimize the Tokhura’s distance-based distortion.

	Application: Used in speech recognition and speaker identification for compressing LPC or cepstral features into a fixed-size codebook.



2. OBJECTIVES
	a. To generate an efficient codebook from the given universe of feature vectors.
	b. To minimize the average distortion using iterative refinement.
	c. To implement codebook splitting using ±ε perturbation until the desired codebook size is achieved.



3. INPUT & OUTPUT
	Type				Description
	Input File			Universe.csv — contains 6340 vectors of 12 coefficients each.
	Output Files		Multiple LBG_Output_<size>.csv files (codebook at each stage).
	Output Data			Final codebook vectors of size K (default = 8).



4. CONSTANTS DEFINED
	Constant	Value		Description
	K			8			Final codebook size (number of clusters).
	P			12			LPC order (number of coefficients per vector).
	S			200			Line buffer size for reading input.
	M			6340		Total number of vectors in the universe.
	E			0.03		Epsilon – used for codebook splitting.
	D			0.00001		Distortion threshold
	WEIGHTS[P]				Used in Tokhura’s distance computation to emphasize certain LPC coefficients.
				{1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0}



3. VARIABLES
	Variable				Description
	vector[M][P]			Stores all input feature vectors.
	codebook[K][P]			Stores current cluster centroids.
	newCodebook[K][P]		Temporary codebook.
	group[M]				Assigns each vector to a cluster.
	freq[K]					Number of vectors in each cluster.
	distance[K]				Average distortion per cluster.
	centroid[K][P]			Temporary centroids during update.
	m						Iteration count.



4. PROGRAM FLOW

	(I) Input Reading (takeInput)
			a. Reads Universe.csv line by line.
			b. Reads each line (12 comma-separated floating-point values).
			c. Loads all vectors into vector[M][P].

	(II) Initial Codebook Creation
			a. Initializes the first codeword to 0:
					codebook[0][j] = 0.0f
			b. The initial codebook size = 1.

	(III) K-Means Clustering (kMeansClustering)
			a. Performs one iteration of K-means clustering:
				For each vector vector[i], compute its Tokhura’s distance to each centroid:
				dist = Σ WEIGHTS[k] * (codebook[j][k] - vector[i][k])²
			b. Assign vector to the nearest cluster.
			c. Update:
				centroid[j] ← sum of all vectors in cluster j
				freq[j] ← number of vectors in cluster j
				distance[j] ← average cluster distortion
			d. Update new codebook:
				codebook[j][k] = centroid[j][k] / freq[j]
			e. Compute total distortion across all clusters.

	(IV) Convergence
			a. Iteratively call kMeansClustering() until:
				|curDist - prevDist| ≤ D
			b. Displays per-iteration distortion and cluster statistics.

	(V) Codebook Splitting (makeCodebook)
			a. Splits each centroid into two slightly perturbed versions:
					newCodebook[2*i]   = codebook[i] + E
					newCodebook[2*i+1] = codebook[i] - E
			b. Doubles the number of clusters each time (1 → 2 → 4 → 8).

	(VI) Iterative Refinement
			a. After each split, kMeansClustering() is called to re-optimize centroids.
			b. Repeats splitting and clustering until K = 8 codewords are formed.



5. KEY FUNCTIONS
	Function			Description
	takeInput()			Reads feature vectors from Universe.csv.
	makeCodebook()		Splits each centroid into two using ±E.
	kMeansClustering()	Performs clustering and computes new centroids with Tokhura’s distance.
	LindeBuzoGray()		Iteratively refines the codebook until convergence.
	storeOutput()		Saves the final codebook for each iteration to CSV.



6. INPUT FILE

	File:
		C:\...\Record Vector\Universe.csv

	Structure:
		0.123,-0.032,0.045,0.210,...,0.004
		0.056,-0.041,0.098,0.182,...,0.007
		...
		Each line represents one 12-dimensional vector.



7. OUTPUT EXAMPLE

	Console Output:
		Turn 4.3:-
		0   :   920   &   15.645912
		1   :   756   &   14.392013
		...
		Distortion: 13.756432

		After plus minus epsilon:
		Codebook_8.csv generated

	Final Output:
		Displays final 8 codewords (each of 12 coefficients).



8. ALGORITHM SUMMARY
	Operation									Result
	Initialize one centroid from universe.		size = 1
	Run K-Means clustering iteratively.			Refined centroid.
	Split centroids using ±E.					Doubles clusters.
	Re-run clustering on new set.				Updated codebook.
	Repeat until size = K (8).					Final codebook ready.



9. EXECUTION STEPS
	a. Save file as LBG.cpp.
	b. Place Universe.csv in the specified directory.
	c. Open Visual Studio 2010 (or later).
	d. Create a Win32 Console Application and add LBG.cpp.
	e. Build and run the program.
	f. Observe the iterative formation of the 8-vector codebook.



11. SUMMARY
	a. Implements LBG algorithm using Tokhura’s distance.
	b. Starts from one centroid and splits iteratively to reach K = 8.
	c. Generates optimized speech vector codebook for vector quantization.
	d. Core step in speech recognition feature compression.


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////