# Pi_Calculation
Used to calculate the value of pi using two methods - Mclauren Series and Monte Carlo. 
Calculates using multiple threads and using a single thread and gives the execution time fot both.

It can be used to calculate the speed up obtained by the use of multi-threading.

The makefile takes the following command:

make all : Will build the executable file for execution

make run thread_count=<No. of threads> n=<Number of Iterations for which you want to calculate the value>

make clean : This will delete the executable file

make plot: This will plot the graph of comparison between the single thread and multi threaded program
