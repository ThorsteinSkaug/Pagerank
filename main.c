#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "read_graph_from_file.c"
#include "PageRank_iterations.c"
#include "top_n_webpages.c"

int main(int argc, char *argv[]){

  //Epsilon from user input
  double epsilon;
  sscanf(argv[2], "%lf", &epsilon);

  //Damping constant from user input
  double d;
  sscanf(argv[3], "%lf", &d);

  //Add n from user input and converting to int
  int n = atoi(argv[4]);

  // Defining the variables used
  int *row_ptr, *col_idx, *N, *edges, *danglers,
  *danglers_idx, *score_idx;
  double *val, *scores;

  // Allocating memory
  N = (int*)malloc(sizeof(int));
  edges = (int*)malloc(sizeof(int));
  danglers = (int*)malloc(sizeof(int));
  danglers_idx = (int*)malloc(sizeof(int));

  // Reading the .txt file and storing with CRS scheme.
  read_graph_from_file(argv[1], N, edges, &row_ptr,
    &col_idx, &val, danglers, &danglers_idx);

  // Allocating memory for scores.
  scores = (double*)malloc(*N*sizeof(double));

  // The pagerank algorithm, giving scores to all webpages.
  PageRank_iterations(*N, *edges, row_ptr, col_idx, val, d, epsilon,
    scores, *danglers, danglers_idx);

  // Defining some new variables needed and allocating memory.
  int *index;
  index = (int*)malloc(*N*sizeof(int));
  double *top_score;
  top_score = (double*)malloc(*N*sizeof(double));

  // Finding the top n most scoring web pages with their indeces and printing them. 
  top_n_webpages(*N, scores,n, &index, &top_score);

  return 0;

}
