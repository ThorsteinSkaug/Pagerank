#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_graph_from_file (char *filename, int *N, int *edges, int **row_ptr,
  int **col_idx, double **val, int *danglers, int **danglers_idx){
  FILE *file = fopen(filename, "r");

/*
Inputs:
filename: Name of file
N: Number of nodes (webpages) in file
edges: Number of edges (links) in file
row_ptr: The row pointer, one of the three vectors used to describe the sparse matrix with the CRS format
col_idx: Column index, one of the other three vectors used to describe the sparse matrix with the CRS format
val: The values in the sparse matrix shown in a 1D array. Their location in the sparse matrix are shown
with the row pointer and the column index
danglers: Is the number of dangling webpages, i.e. pages with no outbound links.
danglers_idx: The index of the danglers.

In this function the web graphs are read and stored using the CRS-format.
*/

  // Skipping the first two lines and storing the number of nodes and edges on the third line
  fscanf(file, "%*[^\n]\n");
  fscanf(file, "%*[^\n]\n");
  fscanf(file, "# Nodes: %d Edges: %d\n", &*N, &*edges);

  // Allocating memory
  *row_ptr = (int*)malloc((*N+1) * sizeof(int));
  *col_idx = (int*)malloc(*edges * sizeof(int));
  *val = (double*)malloc(*edges * sizeof(double));


  // Defining and allocating memory for the from- and to nodes with their indeces to be read from the file
  int *fromNodeId;
  int *toNodeId;
  int fromNode;
  int toNode;

  fromNodeId = (int*)malloc(*edges * sizeof(int));
  toNodeId = (int*)malloc(*edges * sizeof(int));

  // Defining some variables used to read.
  (*danglers) = 0;
  int read = 0;
  int tot = 0;
  int valid = 0;
  // Maximum allowed characters in a line is 256. (Buffersize)
  char line[256];

  // Reading the to- and from nodes and storing
  while (fgets(line, sizeof(line), file)){

    if (fromNode != toNode){
      fromNodeId[valid] = fromNode;
      toNodeId[valid] = toNode;
      valid++;
    }
    read += fscanf(file, "%d %d", &fromNode, &toNode);
  }

  fclose(file);

  // Defining some arrays for number of columns and rows. Allocating as well.
  int *col_count;
  int *row_count;

  col_count = (int*)malloc((*N)*sizeof(int));
  row_count = (int*)malloc((*N)*sizeof(int));


  file = fopen(filename, "r");
  // Skipping and so on
  fscanf(file, "%*[^\n]\n");
  fscanf(file, "%*[^\n]\n");
  fscanf(file, "# Nodes: %d Edges: %d\n", &*N, &*edges);
  fscanf(file, "%*[^\n]\n");

  // Initialising col_count and row_count. Filling arrays with zeros.
  for (int i = 0; i < *N; i++){
    col_count[i] = 0;
  }

  for (int i = 0; i < *N; i++){
    row_count[i] = 0;
  }
  // Reading and counting columns and rows.
  while (fscanf(file, "%d %d", &fromNode, &toNode) != EOF){
    // If the Stanford file is loaded the index start at 1, which must be accounted for.
    // I tried to find other ways of doing this, but nothing else seemed to work.
    // Originally I found the first index first and subtracted it, hoping to make the code
    // more general. Unfortunately it did not work with omp for some reason I could not figure out
    if (strcmp(filename, "web-stanford.txt") == 0){
        fromNode = fromNode - 1;
        toNode = toNode - 1;}

    col_count[fromNode] ++;
    row_count[toNode] ++;

  }
  fclose(file);

    //Finding the number of danglers
    for (int i = 0; i<*N; i++){
      if (col_count[i] == 0){
        (*danglers) ++;
      }
    }
    // Finding the dangler indices.
    int d_idx = 0;
    *danglers_idx = (int*)malloc((*danglers)*sizeof(int));
    for (int i=0; i<*N; i++){
      if (col_count[i] == 0){
        (*danglers_idx)[d_idx] = i;
        d_idx++;
      }
    }

    // Filling the row pointer.
    int tmp = 0;
    for (int i = 0; i < *N+1; i++){
        (*row_ptr)[i] = tmp;
        tmp += row_count[i];
      }

    // Counting the column indeces
    int *col_idx_count;
    col_idx_count = (int*)malloc((*N)*sizeof(int));
    for (int i = 0; i < *N; i++){
      col_idx_count[i] = 0;
    }

    file = fopen(filename, "r");

    // Skipping again
    fscanf(file, "%*[^\n]\n");
    fscanf(file, "%*[^\n]\n");
    fscanf(file, "# Nodes: %d Edges: %d\n", &*N, &*edges);
    fscanf(file, "%*[^\n]\n");

    // same thing here with the stanford file.
    while (fscanf(file, "%d %d", &fromNode, &toNode) != EOF){
      if (strcmp(filename, "web-stanford.txt") == 0){
          fromNode = fromNode - 1;
          toNode = toNode - 1;}

        // CRS MVM. Here the column indeces and val-values are found and filled into their respective arrays.
        (*col_idx)[(*row_ptr)[toNode] + col_idx_count[toNode]] = fromNode;
        (*val)[(*row_ptr)[toNode] + col_idx_count[toNode]] = (double) 1/col_count[fromNode];
        col_idx_count[toNode] ++;

    }

    fclose(file);

}
