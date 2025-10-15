========================================================================
    CONSOLE APPLICATION : kMeans Project Overview
========================================================================

AppWizard has created this kMeans application for you.

This file contains a summary of what you will find in each of the files that
make up your kMeans application.


kMeans.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

kMeans.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

kMeans.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named kMeans.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

1. OVERVIEW
	This program implements the K-Means Clustering Algorithm for generating a codebook of speech feature vectors (e.g., LPC/cepstral coefficients).
	It is typically used in Vector Quantization (VQ) for speech recognition, 
	where a large set of feature vectors (the “universe”) is compressed into representative cluster centroids (codewords).

	Input	: Universe.csv — contains speech feature vectors (each of length 12).
	Output	: Final K = 8 codebook vectors printed to the console.



2. CONSTANTS DEFINED
	Constant	Value		Purpose
	K			8			Number of clusters (codebook size).
	P			12			Dimension of each feature vector (LPC order).
	S			200			Maximum length of each line (buffer).
	M			6340		Total number of vectors in the universe.
	D			0.00001		Convergence threshold for distortion.
	WEIGHTS[P]				Used in Tokhura’s distance computation to emphasize certain LPC coefficients.
				{1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0}
	


3. VARIABLES
	Variable			Description
	vector[M][P]		Stores all input feature vectors.
	codebook[K][P]		Stores current cluster centroids.
	group[M]			Assigns each vector to a cluster.
	freq[K]				Number of vectors in each cluster.
	distance[K]			Average distortion per cluster.
	centroid[K][P]		Temporary centroids during update.
	m					Iteration count.



4. PROGRAM FLOW

	(I) Input Loading (takeInput)
			Reads Universe.csv line by line.
			Each line contains 12 comma-separated floating-point values.
			Loads all M = 6340 vectors into the vector array.

	(II) Initialization (initCodebook)
			The first K = 8 vectors from the universe are selected as initial centroids.

	(III) Clustering Iterations (kMeansClustering)
			a) Performs one iteration of K-means clustering:
				For each vector vector[i], compute its Tokhura’s distance to each centroid:
				dist = Σ WEIGHTS[k] * (codebook[j][k] - vector[i][k])²

			b) Assign vector to the nearest cluster.

			c) Update:
				centroid[j] ← sum of all vectors in cluster j
				freq[j] ← number of vectors in cluster j
				distance[j] ← average cluster distortion

			d) Update new codebook:
				codebook[j][k] = centroid[j][k] / freq[j]

			e) Compute total distortion across all clusters.

	(IV) Convergence
			Iteratively call kMeansClustering() until:
				|curDist - prevDist| ≤ D
			Displays per-iteration distortion and cluster statistics.

	(V) Final Output
			Once converged, prints the final codebook:
				Each row → Codebook index (0–7)
				Each column → Feature dimension (1–12)



5. KEY FUNCTIONS
	Function				Description
	takeInput()				Reads all 12-dimensional feature vectors from Universe.csv.
	initCodebook()			Initializes first 8 vectors as codebook centroids.
	kMeansClustering()		Performs assignment and update steps; returns total distortion.



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
		0   :   788   &   0.004580
		1   :   790   &   0.004891
		...
		Distortion:           0.004782

		Turn 2:-
		...
		Distortion:           0.004756


	Final Codebook:

		0       1       2       3       4       5       6       7
		-------------------------------------------------------------
		0.321   0.284   0.177   0.210   0.205   0.198   0.223   0.249
		...



8. ALGORITHM SUMMARY
	Stage				Description
	Initialization		Choose 8 initial centroids from dataset.
	Assignment			Assign each vector to nearest centroid (Tokhura distance).
	Update				Compute new centroids by averaging cluster members.
	Convergence			Repeat until change in total distortion ≤ 0.00001.



9. EXECUTION STEPS
		Save the file as kMeans.cpp.
		Place Universe.csv in the specified directory.
		Open Visual Studio 2010 or later.
		Create a Win32 Console Application.
		Add kMeans.cpp to the project and build.
		Run to obtain the final codebook.



10. SUMMARY
		Implements K-Means clustering using Tokhura’s distance for speech feature quantization.
		Generates 8 codebook vectors, each of 12 dimensions.
		Used in Vector Quantization stages of speech recognition pipelines.

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////