#include <stddef.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include"header.h"

long thread_count;
long long n;
double sum;
sem_t sem;




/*------------------------------------------------------------------
 * Function:    Get_args
 * Purpose:     Get the command line args
 * In args:     argc, argv
 * Globals out: thread_count, n
 */
void Get_args(int argc, char* argv[]) {
   thread_count = strtol(argv[1], NULL, 10);
   n = strtoll(argv[2], NULL, 10);
}


// const double micro = 1.0e-6;

// void* Thread_sum(void* rank);
// void *calculate(void *threadid);


int main(int argc, char* argv[])
{
	double t1,t2,pi;

   FILE *fptr1;
   FILE *fptr2;


	/*---------------------Mclauren Serial and Parallel Evaluation-------------------------*/
	long thread;  /* Use long in case of a 64-bit system */
   pthread_t* thread_handles;
   double start, finish;


   /* Get number of threads from command line */
   Get_args(argc, argv);

   thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t)); 
   /* 0:  share sem among threads, not processes */
   /* 1:  initial value of sem is 1 */
   sem_init(&sem, 0, 1);
   t1 = calculateTime();
   sum = 0.0;
   for (thread = 0; thread < thread_count; thread++)  
      pthread_create(&thread_handles[thread], NULL,Thread_sum, (void*)thread);  

   for (thread = 0; thread < thread_count; thread++) 
      pthread_join(thread_handles[thread], NULL); 
   sum = 4.0*sum;
   t2 = calculateTime();

   printf("   The value of pi using mclauren series in threaded program = %.15f\n", sum);
   printf("The elapsed time is %lf seconds\n", t2 - t1);

   

   t1 = calculateTime();
   sum = Serial_pi(n);
   t2 = calculateTime();

   fptr1 = fopen("./Mclauren.dat","a");
   fprintf(fptr1,"%li  %lf\n",thread_count,t2-t1);
   fclose(fptr1);


   printf(" The value of pi using serial evaluation of pi  = %.15f\n", sum);
   printf("The elapsed time is %lf seconds\n", t2 - t1);
   
   sem_destroy(&sem);
   free(thread_handles);


   /*-----------------Mclauren Series Evaluation finishes-----------------------------*/










	/*-------------------------Monte Carlo Serial Evaluation-------------------------*/

	double pi_monte_serial;
	t1 = calculateTime();
	pi_monte_serial = Serial_pi_monte(n);
	t2 = calculateTime();
	t2 = t2-t1;
	printf("\n The value of pi using serial MONTE CARLO method =  %.15f",pi_monte_serial);
	printf("\n And the time taken for it =  %.lf",t2);

	/*-----------------------Monte Carlo Serial Evaluation finished---------------*/
	t1 = calculateTime();
	pthread_t threads[thread_count];
   int rc;
   long t;
   void *status;
   double tot_in=0;
   
   
   
   for(t=0;t<thread_count;t++){
     rc = pthread_create(&threads[t], NULL, calculate, (void *)t);                   //check it again
     if (rc){
       printf("ERROR; return code from pthread_create() is %d\n", rc);
       exit(-1);
       }
     }

   //join the threads
   for(t=0;t<thread_count;t++){
           
      pthread_join(threads[t], &status);
      //printf("Return from thread %ld is = %f\n",t, *(double*)status);
      
      tot_in+=*(double*)status;            //keep track of the total in count
     
     }

     pi = ((double)tot_in/(double)n)*4;
     t2 = calculateTime();
     t2 = t2-t1;
     printf("\n The value of PI using parallel Monte Carlo = %.15lf \n", pi);
     printf("\n The time taken for Monte Carlo Parallel calculation = %lf",t2);

     fptr2 = fopen("./Monte-Carlo.dat","a");
   fprintf(fptr2,"%li  %lf\n",thread_count,t2);
   fclose(fptr2);
   
   /* Last thing that main() should do */
   pthread_exit(NULL);

   /*----------------Monte Carlo Parallel Evaluation Finishes-------------------------*/

   return 0;
}