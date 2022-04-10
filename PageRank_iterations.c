#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <math.h>

void PageRank_iterations (int N, int edges, int *row_ptr, int *col_idx, double *val, double
d, double epsilon, double *scores, int danglers, int *danglers_idx){

/*
Inputs:
N: Number of nodes (webpages) in file
edges: Number of edges (links) in file
row_ptr: The row pointer, one of the three vectors used to describe the sparse matrix with the CRS format
col_idx: Column index, one of the other three vectors used to describe the sparse matrix with the CRS format
val: The values in the sparse matrix shown in a 1D array. Their location in the sparse matrix are shown
with the row pointer and the column index
d: The damping factor. Usually 0.85. Written in by user
epsilon: The threshold value for when we have converged to a score. From user input.
scores: Score given to each webpage found here.
danglers: Is the number of dangling webpages, i.e. pages with no outbound links.
danglers_idx: The index of the danglers.

This function uses the danglers and the CRS stored sparse matrix to score each webpage and with an
iterative process of converging towards the correct value.
*/

// Defining some variables used to calculate the damping, as well as defining some temp values used.
double W = 0, Ndiv = 1.0/ (double) N, timesd = Ndiv*d, damper = Ndiv-timesd, tmpdiff, diff = 1.;
// Allocating memory to the temporary values used below.
double *temp = (double*) malloc(edges*sizeof(double));

// Initialising scores.
for(int i=0; i<N;i++){
  scores[i] = Ndiv;
}
// Factors in the damping factor on the values from the CRS scheme.
for(int i =0; i<edges;i++){
  val[i] *= d;
}

// Here the scores are found by convergence with an iterative process.
// The loop stops when the difference between the previous score and the next is smaller than the threshold value
int idx = 0;
while(diff > epsilon){
  for (int i = 0; i < danglers;i++){
  W += scores[danglers_idx[i]];
  }
  W = W*timesd + damper;
  idx = 0;

  // The comuptationally "heavy" part of the alorithm is here parallelised using all available threads.
  #pragma omp parallel for
  for (int i = 0; i < N; i++) {
    temp[i] = W;
    for (int j = row_ptr[i]; j < row_ptr[i+1]; j++){
      temp[i] += val[j] * scores[col_idx[j]];
    }
  }
  diff = temp[0]-scores[0];
  for (int i=0; i<N; i++){
    // Finding the differnece between the old and new score, to see if the treshold value is reached.
    tmpdiff = fabs(temp[i]-scores[i]);
    if (tmpdiff > diff){
    diff = tmpdiff;
    }
    scores[i] = temp[i];
  }
  }
}
