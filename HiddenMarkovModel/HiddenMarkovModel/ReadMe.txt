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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

1. OVERVIEW
    Implements an HMM toolkit that:
    a. Loads HMM parameters (A, B, P) and an observation sequence (O).
    b. Computes observation likelihood using the Forward algorithm.
    c. Computes backward probabilities (Backward algorithm).
    d. Optionally computes the most likely state sequence (Viterbi).
    e. Re-estimates HMM parameters using Baum–Welch (EM) and writes final matrices.



2. OBJECTIVES
    a. Evaluate P(O∣λ) for a trained model.
    b. Re-train (refine) HMM parameters from a single observation sequence via Baum–Welch.
    c. Provide utilities to inspect and persist model matrices.



3. INPUT and OUTPUT
    Type	        Description
    Input Files	    - HMM_MATRIX_A.csv – State transition matrix (N × N)
                    - HMM_MATRIX_B.csv – Observation probability matrix (N × M)
                    - HMM_MATRIX_P.csv – Initial state probabilities (1 × N)
                    - HMM_OBSERVATION_SEQUENCE.csv – Observation sequence (1 × T)
    
    Output	        Probability value (P(O∣λ))
                    - HMM_FINAL_P.csv
                    - HMM_FINAL_A.csv
                    - HMM_FINAL_B.csv



4. CONSTANTS DEFINED
    Symbol  Value   Meaning
    N	    5       Number of hidden states
    M	    32      Number of observation symbols
    T	    85      Length of observation sequence



5. GLOBAL DATA STRUCTURES
    double A[N][N]  — state transition probability matrix a[i][j]
    double B[N][M]  — observation probability matrix b[j](k).
    double P[N]     — initial state distribution π[i]
    int O[T]        — observation sequence (0-indexed symbols).
            


6. KEY FUNCTIONS
    Function	            Description
    readAMatrix()	        Reads transition probabilities (A) from CSV file.
    readBMatrix()	        Reads observation probabilities (B) from CSV file.
    readPMatrix()	        Reads initial state probabilities (P) from CSV file.
    readOMatrix()	        Reads observation sequence (O) from CSV file.
    makeUnbiasedMatrix()    Set uniform initial values for P, A, B (fallback).
    finalMatrix()           Write P, A, B into CSV files.

    forwardProcedure()      Compute forward probabilities alpha[t][i] and return P(O∣λ).
        a. Initialization:      alpha[0][i] = P[i]*B[i][O[0]].
        b. Induction:           alpha[t][j] = (Σ_i alpha[t-1][i]*A[i][j]) * B[j][O[t]].
        c. Termination:         Σ_i alpha[T-1][i].

    backwardProcedure()     Compute backward probabilities beta[t][i].
        a. Initialization:      beta[T-1][i] = 1.
        b. Induction:           beta[t][i] = Σ_j A[i][j]*B[j][O[t+1]]*beta[t+1][j].

    viterbiProcedure()      Compute most likely state sequence and its probability using Viterbi.
        a. Initialization:      delta[0][i] = P[i]*B[i][O[0]]; 
        b. Induction:           recursion uses max over previous states; backtrace via psi.

    baumWelchProcedure      Single EM (Baum–Welch) re-estimation step:
        a. Initialization:
                                xi[t][i][j] = alpha[t][i] * A[i][j] * B[j][O[t+1]] * beta[t+1][j]
                                (normalize by sum).
                                gamma[t][i] = Σ_j xi[t][i][j] 
                                (and last gamma[T-1] from alpha).
        b. Re-estimate:
                                P[i] = gamma[0][i].
                                A[i][j] = Σ_t xi[t][i][j] / Σ_t gamma[t][i].
                                B[j][k] = Σ_{t:O[t]==k} gamma[t][j] / Σ_t gamma[t][j].



7. PROGRAM FLOW
    a. Read A, B, P, O via respective readers (or set uniform matrix).
    b. printMatrix() — inspect initial model.
    c. Compute probability = forwardProcedure(alpha).
    d. Compute beta via backwardProcedure(beta).
    e. Optionally compute Viterbi path (viterbiProcedure) — code present but commented out.
    f. Loop (example: 10 iterations shown):
        i. Call baumWelchProcedure(alpha, beta, gamma, xi) to re-estimate A, B, P.
        ii. Recompute forward/backward and print updated probability and matrices.
    g. Write final matrices with finalMatrix().



8. CONSOLE OUTPUT
    a. Initial matrix printout: P, rows of A and B, column sums of B.
    b. P(O | λ) printed after forward pass.
    c. After each Baum–Welch iteration: updated matrices and P(O | λ).
    d. Final CSV files: HMM_FINAL_P.csv, HMM_FINAL_A.csv, HMM_FINAL_B.csv.



9. APPLICATION
    a. Speech recognition probability evaluation.
    b. Pattern likelihood estimation in acoustic modeling.
    c. Sequence analysis in bio-informatics and signal processing.
    d. Training and validation of HMM parameters.

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////