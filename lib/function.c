#include <stddef.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>

double sum;
sem_t sem;
long thread_count;
long long n;


// /*------------------------------------------------------------------
//  * Function:    Get_args
//  * Purpose:     Get the command line args
//  * In args:     argc, argv
//  * Globals out: thread_count, n
//  */
// void Get_args(int argc, char* argv[]) {
//    if (argc != 3) Usage(argv[0]);
//    thread_count = strtol(argv[1], NULL, 10);  
//    if (thread_count <= 0) Usage(argv[0]);
//    n = strtoll(argv[2], NULL, 10);
//    if (n <= 0) Usage(argv[0]);
// }  /* Get_args */

// ------------------------------------------------------------------
//  * Function:  Usage
//  * Purpose:   Print a message explaining how to run the program
//  * In arg:    prog_name
 
// void Usage(char* prog_name) {
//    fprintf(stderr, "usage: %s <number of threads> <n>\n", prog_name);
//    fprintf(stderr, "   n is the number of terms and should be >= 1\n");
//    fprintf(stderr, "   n should be evenly divisible by the number of threads\n");
//    exit(0);
// }  /* Usage */

/*-------------------------------------------------*/

/* This will be used to measure time */


const double micro = 1.0e-6;
double calculateTime()
{
 struct timeval TV;
 struct timezone TZ;
 int RC = gettimeofday(&TV,&TZ);
 if (RC == -1){
 printf("Bad call to gettimeofday\n");
 return(-1);
 }
 return ( ((double)TV.tv_sec ) + micro * ((double) TV.tv_usec
));
}

/* Will be used in Mclauren Series evaluation of pi*/

void* Thread_sum(void* rank) {
   long my_rank = (long) rank;
   double factor;
   long long i;
   long long my_n = n/thread_count;
   long long my_first_i = my_n*my_rank;
   long long my_last_i = my_first_i + my_n;
   double my_sum = 0.0;

   if (my_first_i % 2 == 0)
      factor = 1.0;
   else
      factor = -1.0;

   for (i = my_first_i; i < my_last_i; i++, factor = -factor) {
      my_sum += factor/(2*i+1);
   }
   sem_wait(&sem);
#  ifdef VERBOSE
   printf("Thread %ld > Obtained semaphore\n", my_rank);
   fflush(stdout);
#  endif
   sum += my_sum;
   sem_post(&sem);

   return NULL;
}

/* Serial pi calculation using Mclauren */

double Serial_pi(long long n) {
   double sum = 0.0;
   long long i;
   double factor = 1.0;

   for (i = 0; i < n; i++, factor = -factor) {
      sum += factor/(2*i+1);
   }
   return 4.0*sum;

}




/*------------------Monte Carlo Functions Begin Here-----------------*/

double RandomGen(double a, double b) {
    double random = ((double) rand()) / (double) RAND_MAX;
    double diff = b - a;
    double r = random * diff;
    return a + r;
}


double calculateDist(double a, double b)
{
    double sum,dist;
    a = a*a;
    b = b*b;

    sum = a+b;
    dist = sqrt(sum);
    return dist;
}



double Serial_pi_monte(double n)
{
	long long int i;
	double dist,xc,yc,pi;
	double inside = 0;

	for(i=0 ; i<n ; i++)
	{
		xc = RandomGen(-1,1);
		yc = RandomGen(-1,1);

		dist = calculateDist(xc,yc);
		if(dist < 1)
		{
			inside++;
		}

	}

	pi = inside/n;
	pi = pi*4;

	return pi;

}


/*-----------------Serial Monte Carlo Finish---------------------------------*/


/*---------------------------Parallel Monte Carlo------------------------*/

double randomGenerator(){

   int random_value = rand(); //Generate a random number   
   double random = random_value / (double) RAND_MAX; 
   return random;
}

void *calculate(void *threadid)
{
   long longTid;
   longTid = (long)threadid;
   
   int tid = (int)longTid;       

   double *circle = (double *)malloc(sizeof(double));
   *circle=0;
   
   //get the total number of n for a thread
   double t_iter= n/thread_count;
   
   int i=0;
   
   //calculation
   for(i=0;i<t_iter;i++){
      double xc = randomGenerator();
      double yc = randomGenerator();
      
      double result = sqrt((xc*xc) + (yc*yc));
      
      if(result<1){
         *circle+=1;         //check if the generated value is inside a unit circle
      }
      
   }
   
   //get the remaining n calculated byc thread 0
   // if(tid==0){
   //    double remainder = n%thread_count;
      
   //    for(i=0;i<remainder;i++){
   //    double xc = randomGenerator();                         /*Read it properly*/
   //    double yc = randomGenerator();
      
   //    double result = sqrt((xc*xc) + (yc*yc));
      
   //    if(result<1){
   //       *circle+=1;         //check if the generated value is inside a unit circle
   //    }
      
   // }
   // }


   //printf("In count from #%d : %f\n",tid,*circle);
   
   pthread_exit((void *)circle);     //return the in count
}


double thread_pi_monte(long NUM_THREADS, long long iterations)
{
	pthread_t threads[NUM_THREADS];
   int rc;
   long t;
   void *status;
   double tot_in=0;
   
   
   
   for(t=0;t<NUM_THREADS;t++){
     rc = pthread_create(&threads[t], NULL, calculate, (void *)t);
     if (rc){
       printf("ERROR; return code from pthread_create() is %d\n", rc);
       exit(-1);
       }
     }

   //join the threads
   for(t=0;t<NUM_THREADS;t++){
           
      pthread_join(threads[t], &status);
      //printf("Return from thread %ld is : %f\n",t, *(double*)status);
      
      tot_in+=*(double*)status;            //keep track of the total in count
     
     }

     double pi;
     pi = ((double)tot_in/(double)iterations)*4;
     printf("%lf \n", pi);
   
   /* Last thing that main() should do */
   pthread_exit(NULL);
}


/*----------------------Monte Carlo Parallel finish--------------------------------*/