#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

void top_n_webpages(int N, double* scores, int n, int **index, double **top_score) {

/*
Inputs:
N: Number of nodes (webpages) in file
scores: Score given to each webpage found in PageRank_iterations.c
n: The top n scores for the webpages will be found here. So n gives extent of the top scorer list. User given.
index: Indices for the top scores
top_score: The scores among the n highest scores.

This function find the top n scores found in PageRank_iterations.c by looping through all scores,
finding the highest remaining score n times.
*/

// Looping through all scores n times.
for (int i=0; i<n; i++)
 {
   // Defining some temporary values used for calculation
   double max_val = scores[i];
   int max_idx = i;
   // Parallising here, as it is the most demading part of the algorithm. Uses all available threads.
   #pragma omp parallel for
   for (int j=0; j<N; j++) {
     if (scores[j] > max_val) {
       max_val = scores[j];
       max_idx = j;
     }
   }
   (*top_score)[i] = scores[max_idx];
   (*index)[i] = max_idx;
   scores[max_idx] = 0;
 }

// Printing the top n scores with their respective indeces.
 printf("\nIndex\tScore\n");
 printf("===================\n");
 for (int i = 0; i < n; i++){
     printf("%d:\t%f\n", (*index)[i], (*top_score)[i]);
 }
 printf("===================\n");

}
