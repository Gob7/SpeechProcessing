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
	a vector quantization technique used to generate a compact codebook from a large set of speech feature vectors.
	The LBG algorithm starts with a single centroid and iteratively splits and refines clusters using Tokhura’s distance 
	until the required number of codewords (K = 8) is obtained.

	Application: Used in speech recognition and speaker identification for compressing LPC or cepstral features into a fixed-size codebook.



2. CONSTANTS DEFINED
	Constant	Value	Description
	K			8		Final codebook size (number of clusters).
	P			12		LPC order (number of coefficients per vector).
	S			200		Line buffer size for reading input.
	M			6340	Total number of vectors in the universe.
	E			0.03	Epsilon – used for codebook splitting.
	WEIGHTS[P]			Used in Tokhura’s distance computation to emphasize certain LPC coefficients.
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



4. PROGRAM FLOW

	(I) Input Reading (takeInput)
			a) Reads Universe.csv line by line.
			b) Reads each line (12 comma-separated floating-point values).
			c) Loads all vectors into vector[M][P].

	(II) Initial Codebook Creation
			a) Initializes the first codeword using the first vector of the universe:
					codebook[0][j] = vector[0][j]
			b) The initial codebook size = 1.

	(III) K-Means Clustering (kMeansClustering)
			a) For each vector:
					Computes Tokhura’s distance between the vector and each codeword.
					Assigns the vector to the nearest centroid.
			b) Computes new centroids by averaging vectors in each cluster.
			c) Calculates average distortion for convergence checking.
			d) Updates the codebook with new centroid values.

	(IV) Codebook Splitting (makeCodebook)
			a) Splits each centroid into two slightly perturbed versions:
					newCodebook[2*i]   = codebook[i] + E
					newCodebook[2*i+1] = codebook[i] - E
			b) Doubles the number of clusters each time (1 → 2 → 4 → 8).

	(V) Iterative Refinement
			a) After each split, kMeansClustering() is called to re-optimize centroids.
			b) Repeats splitting and clustering until K = 8 codewords are formed.



5. KEY FUNCTIONS
	Function			Description
	takeInput()			Reads feature vectors from Universe.csv.
	makeCodebook()		Splits each centroid into two using ±E.
	kMeansClustering()	Performs one iteration of the K-Means algorithm with Tokhura’s distance.



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
		Turn 1:-
		0   :   6340   &   0.000000
		Distortion:             0.000000

		Turn 2:-
		0   :   3170   &   0.003450
		1   :   3170   &   0.003487
		Distortion:             0.003469
		...
		Turn 8:-
		0   :   790   &   0.004521
		1   :   795   &   0.004512
		...
		Distortion:             0.004485


	Final Output:
		Displays final 8 codewords (each of 12 coefficients).



8. ALGORITHM SUMMARY
	Operation									Result
	Initialize one centroid from universe.		size = 1
	Run K-Means clustering.						Refined centroid.
	Split centroids using ±E.					Doubles clusters.
	Re-run clustering on new set.				Updated codebook.
	Repeat until size = K (8).					Final codebook ready.



9. EXECUTION STEPS
	a) Save file as LBG.cpp.
	b) Place Universe.csv in the specified directory.
	c) Open Visual Studio 2010 (or later).
	d) Create a Win32 Console Application and add LBG.cpp.
	e) Build and run the program.
	f) Observe the iterative formation of the 8-vector codebook.



11. SUMMARY
	a) Implements LBG algorithm using Tokhura’s distance.
	b) Starts from one centroid and splits iteratively to reach K = 8.
	c) Generates optimized speech vector codebook for vector quantization.
	d) Core step in speech recognition feature compression.


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////