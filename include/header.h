#include <stddef.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
/*for calculating time */ 


double calculateTime();


/*from mclauren*/


double Serial_pi(long long n);
void* Thread_sum(void* rank);

double Serial_pi(long long n);
double thread_pi_mclauren(long thread_count,long long n);




/*Monte Carlo functions*/

double RandomGen(double a, double b);
double calculateDist(double a, double b);
double Serial_pi_monte(double n);
double randomGenerator();

void *calculate(void *threadid);
double thread_pi_monte(long NUM_THREADS, long long iterations);

