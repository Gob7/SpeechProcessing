========================================================================
    CONSOLE APPLICATION : HiddenMarkovModel Project Overview
========================================================================

AppWizard has created this HiddenMarkovModel application for you.

This file contains a summary of what you will find in each of the files that
make up your HiddenMarkovModel application.


HiddenMarkovModel.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

HiddenMarkovModel.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

HiddenMarkovModel.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named HiddenMarkovModel.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

1. OVERVIEW
    This program implements the Forward Algorithm of the Hidden Markov Model (HMM) 
    to compute the likelihood probability P(O∣λ) of a given observation sequence for a trained model.
    
    It uses three HMM parameters 
    — State Transition Matrix (A), Observation Probability Matrix (B), and Initial State Probability Matrix (P) 
    — along with an observation sequence (O) to evaluate how well the model explains the sequence.



2. OBJECTIVES
    a. To load HMM parameters (A, B, P) and observation sequence (O) from files.
    b. To apply the Forward Procedure for computing P(O∣λ).
    c. To validate speech pattern likelihood in recognition systems.



3. iNPUT and OUTPUT
    Type	        Description
    Input Files	    - HMM_A_MATRIX.csv – State transition matrix (N × N)
                    - HMM_B_MATRIX.csv – Observation probability matrix (N × M)
                    - HMM_P_MATRIX.csv – Initial state probabilities (1 × N)
                    - HMM_OBSERVATION_SEQUENCE.csv – Observation sequence (1 × T)
    Output	        Probability value (P(O∣λ))



4. CONSTANTS DEFINED
    Symbol  Value   Meaning
    N	    5       Number of hidden states
    M	    32      Number of observation symbols
    T	    85      Length of observation sequence



5. ALGORITHM: Forward Procedure

	(I) Initialization
            For each state i: 
            α[1][i] = pi[i] * b[i][O[1]]

	(II) Induction
            For t=2,3,…,T and for each state j:
            α[t][j] = {Σ α[t-1][i] * a[i][j]} * b[j][O[t]]

	(III) Termination
            The final probability is:
            P(O∣λ) = Σ α[T][i]
            


6. KEY FUNCTIONS
    Function	        Description
    readAMatrix()	    Reads transition probabilities (A) from CSV file.
    readBMatrix()	    Reads observation probabilities (B) from CSV file.
    readPMatrix()	    Reads initial state probabilities (P) from CSV file.
    readOMatrix()	    Reads observation sequence (O) from CSV file.
    forwardProcedure()	Implements the forward algorithm to compute (P(O∣λ))



7. Flow of Execution
    a. Start the program.
    b. Load all HMM matrices (A, B, P, and O).
    c. Initialize the α (alpha) matrix.
    d. Perform recursive computation using transition and observation probabilities.
    e. Sum the final α values to compute overall probability.
    f. Display P(O∣λ).



8. Example Console Output
    O[0] = 11
    a[0][0] = 0.024560 = 0.5 * 0.04912
    a[0][1] = 0.015240 = 0.3 * 0.05080
    ...
    a[84][0] = 0.000000012
    a[84][1] = 0.000000018
    a[84][2] = 0.000000010
    a[84][3] = 0.000000007
    a[84][4] = 0.000000009

    P(O | y) = 0.000000056



9. Applications
    a. Speech recognition probability evaluation.
    b. Pattern likelihood estimation in acoustic modeling.
    c. Sequence analysis in bio-informatics and signal processing.
    d. Training and validation of HMM parameters.

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////