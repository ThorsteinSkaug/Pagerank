**Running the project:**
To compile the project, run the following command:

gcc -o main -fopenmp main.c

After compilation, the program can be executed with user inputs:

./main Filename epsilon d n

Where you need to implement the desired variables.

Filename is the name of the web graph to be analysed. In this project, this is limited to one of the following:
simple_webgraph.txt
100nodes_graph.txt
web-stanford.txt
web-notredame.txt

epsilon is the threshold value. This is usually vey small, for example 0.000001

d is the damping factor. Usually 0.85. Set to 1 if you want to ignore damping.

n is the number of webpages included in the top scoring list attained after running the project. Example value: 10.




**Comments**
There is definitely room for improvement here. Mainly I have not implemented any header files, which I know is unfortunate. I did not feel it was
necessary here, but it should still have been included. I could also have made function files, which would have given the project better structure.
The parallelisation did not have much effect here, but I can see how it is vital for real life web graphs with millions of nodes. I did also ignore
making a Makefile, which would also have been an improvement. 
