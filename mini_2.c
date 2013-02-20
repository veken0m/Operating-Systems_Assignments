/*
 * Prints out the location of various memory segments of the program:
 * TEXT, BSS, DATA, HEAP, STACK and MMAP
 *
 * Build: gcc -o mini_2 mini_2.c -pthread
 * Usage: ./mini_2
 */

/*  
  - Mini Assignment 2 -
  Author: Michael Lagace
  Student ID: 260372922
  Date: Feb 19, 2013
  Course: Operating Systems - ECSE 427
  Instructor: Prof. Maheswaran
 */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define N 100000

int itemCount = 0;
int totalProduced = 0;
int itemsConsumed = 0;
int queue[10*N];
pthread_mutex_t lock;
pthread_cond_t cond;
int locked;
 
struct timeval  tv1, tv2;



static void * consumerThread(void *arg)
{
	char *s = (char *) arg;
	printf("%s\n", s);	
	while(itemsConsumed != 10*N){
		// lock mutex
		pthread_mutex_lock(&lock);
		// If no item to be consumed, wait
		while(itemCount == 0){
			printf("Queue is empty, consumer waiting...\n");
			pthread_cond_wait(&cond,&lock);
		}
		// get value and increment counters
		int getValue = queue[itemsConsumed];
		//printf("Consumed: %i\n", getValue);
		itemCount--;
		itemsConsumed++;
		//printf("Total items consumed: %i\n",itemsConsumed); 
		// unlock mutex
		pthread_mutex_unlock(&lock);
	}
		// Once done, calculate run time
		gettimeofday(&tv2, NULL);
     		printf ("Total time = %f seconds\n",
             	(double) (tv2.tv_usec - tv1.tv_usec)/1000000 +
             	(double) (tv2.tv_sec - tv1.tv_sec));
		exit(0);
}

static void * producerThread(void *arg)
{
	int itemProduced = 0;
	int i = (int *) arg;
	printf("Producer %i created\n", i);	
	while(itemProduced < N){
			// While thread did not produce 10*N items, produce items
			pthread_mutex_lock(&lock); // lock mutex
			//printf("Produced: %i\n",queue[totalProduced] = rand()%10+1);
			// Increment counters
			totalProduced++;			
			itemProduced++;
			itemCount++;
			pthread_cond_signal(&cond);  // signal consumer thread
			pthread_mutex_unlock(&lock); // unlock mutex	
	}
}

int main(int argc, char *argv[]) {

	gettimeofday(&tv1, NULL);
	pthread_t c0;
	pthread_t p0[9];
	// initialize mutex and condition variable	
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&cond, NULL);
       
	int i;
	// create the consumer thread
	pthread_create(&c0, NULL, consumerThread, "Consumer created");
	// create 10 producer threads
	for(i = 0; i < 10; i++)
        {             
		pthread_create(&p0[i], NULL, producerThread, (void *) i);
        }

	pthread_join(c0, NULL);
	for(i = 0; i < 10; i++)
        {             
                pthread_join(p0[i], NULL);
        }
}
