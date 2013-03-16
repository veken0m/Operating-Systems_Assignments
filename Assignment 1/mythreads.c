/*
 *
 * Build: gcc my-test.c mythreads.c -DHAVE_PTHREAD_RWLOCK=1 -lslack -lm -lrt -o my-test
 * Usage: ./my-test
 */

/*  
  - Assignment 1 -
  Author: Michael Lagace
  Student ID: 260372922
  Date: March 05, 2013
  Course: Operating Systems - ECSE 427
  Instructor: Prof. Maheswaran
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include "mythreads.h"

// MAX VALUES
#define MAX_THREADS 16
#define MAX_SEMAPHORES 6

// THREAD STATES 
#define ERROR -1
#define EXIT 0
#define WAIT 1
#define RUNNABLE 2
#define RUNNING 3

static ucontext_t default_context;
static sigset_t block_signal;
static List *runqueue;

static mythread_control_block table[MAX_THREADS];
static mythread_semaphore semaphores[MAX_SEMAPHORES];

static int thread_index;
static int quantums;
static int semaphore_index;
static int running_thread;
static int thread_done;
static int scheduler_count;

/*
* Initializes all the global data structures for the thread system
*/
int mythread_init(){
	runqueue = list_create(NULL);
	quantums = 250;
	running_thread = ERROR;
	semaphore_index = 0;
	thread_index = 0;
	scheduler_count = 0;
	thread_done = 0;

	int i;
	for(i = 0; i<MAX_THREADS;i++){
		table[i].state=ERROR;
	}

	for(i = 0; i<MAX_SEMAPHORES;i++){
		semaphores[i].in_use=0;
	}

	sigemptyset(&block_signal);
	sigaddset(&block_signal, SIGALRM);
}

/*
* Creates a new thead. Returns an integer that points to the thread control
* block that is allocated to the newly created thread.
*
* - Allocates stack and user context
* - New thread starts running threadfunc when it starts
* - Thread name and id is stores in control block
* - Sets state to RUNNABLE and add to runqueue
*/

int mythread_create(char *threadname, void (*threadfunc)(), int stacksize){
	if (getcontext(&table[thread_index].context) == -1)
		printf("Error while creating new thread\n");

	// Assign a stack to the thread
	table[thread_index].thread_name = threadname;
	table[thread_index].thread_id = thread_index;
	table[thread_index].run_time = 0;
	table[thread_index].context.uc_stack.ss_size = stacksize;
	table[thread_index].context.uc_stack.ss_sp = malloc(stacksize);
	table[thread_index].context.uc_link = &default_context;
	// Set thread state to RUNNABLE and set the context to the threadfunc
	table[thread_index].state = RUNNABLE;
	makecontext(&table[thread_index].context, threadfunc, 0);
	runqueue = list_append_int(runqueue, thread_index);

	return thread_index++;
}

/*
* Called when a thread is done running. 
*
* - Set thread state to EXIT
*/
void mythread_exit(){
	table[running_thread].state=EXIT;
}

/*
* Swtiches control from the main thread to one of the threads
* in the runqueue
*
* - Activates the thread switcher scheduler() to switch
* context every quantum
*/
void runthreads(){

	// Set a timer to call to scheduler every quantum ms
	sigset(SIGALRM, scheduler);
	struct itimerval clock;
	clock.it_interval.tv_sec = 0;
	clock.it_interval.tv_usec = quantums;
	clock.it_value.tv_sec = 0;
	clock.it_value.tv_usec = quantums;
	setitimer(ITIMER_REAL, &clock, 0);

	if(!list_empty(runqueue)){
		int nThread = list_shift_int(runqueue);
		running_thread = nThread;
		table[running_thread].state = RUNNING;
	    swapcontext(&default_context, &table[nThread].context);
	} else {
		printf("Runqueue is empty!");
	}
	while(!thread_done);
}

/*
* Sets the quantum size for the round robin scheduler 
*/
void set_quantum_size(int quantum){
	quantums = quantum;
}

/*
* Creates a semaphore and set its initial value to the given parameter.
* 
* - Semaphore table holds the state of the semaphores and contains a queue 
*  hold the threads waiting on the semaphore (wait_list)
*/
int create_semaphore(int value){

	semaphores[semaphore_index].value = value;
	semaphores[semaphore_index].initial_value = value;
	semaphores[semaphore_index].in_use = 1;
	semaphores[semaphore_index].wait_list = list_create(NULL);

	return semaphore_index++;
}

/*
* When a threads called this function, value of semaphore in decremented, if the
* value goes below 0 the thread is put in a WAIT state. Consequently the thread is
* taken out of the runqueue if semaphore < 0
*/
void semaphore_wait(int semaphore){
	int previous_count = scheduler_count;

	sigprocmask(SIG_BLOCK, &block_signal, NULL);
	semaphores[semaphore].value--;

	if(semaphores[semaphore].value<0){
		list_append_int(semaphores[semaphore].wait_list, running_thread);
		table[running_thread].state = WAIT;
	}
	sigprocmask(SIG_UNBLOCK, &block_signal, NULL);

	while(scheduler_count == previous_count);
}


/*
* When a thread calls this function, the value of the semaphore is incremented. If
* value is not greater than 0, then we should have at least one thread in the wait_list.
* The thread at the top of the waiting queue is dequeued and enqueue into the runqueue
*
* - The state of the next thread is then changed to RUNNABLE
*/
void semaphore_signal(int semaphore){
	if(semaphore > semaphore_index){
		printf("Wrong semaphore");
	}
	if(semaphores[semaphore].in_use <0){
		printf("Semaphore not active");
	}
	semaphores[semaphore].value++;

	if(semaphores[semaphore].value <= 0){
		int next_thread = list_shift_int(semaphores[semaphore].wait_list);
		table[next_thread].state = RUNNABLE;
		list_append_int(runqueue, next_thread);
	}

}

/*
* Removes a semaphore from the system if threads are not waiting on it.
*/
void destroy_semaphore(int semaphore){

	if(semaphores[semaphore].initial_value != semaphores[semaphore].value){
		printf("Semaphore value has changed!\n");
	}
	if(list_empty(semaphores[semaphore].wait_list)){
		semaphores[semaphore].in_use = 0;
	} else {
		printf("Threads still waiting on semaphore! Cannot destroy semaphore.\n");
	}
}

/*
* Prints the state of all the threads maintained by the mythread library
*/
void mythread_state(){
	char* state_msg;
	int i;
	for(i = 0; i<MAX_THREADS; i++){
		if(table[i].state != ERROR){
			switch(table[i].state){
				case WAIT: state_msg = "WAIT";
				break;
				case RUNNABLE: state_msg = "RUNNABLE";
				break;
				case RUNNING: state_msg = "RUNNING";
				break;
				case EXIT: state_msg = "EXIT";
				break;
			}
			sigprocmask (SIG_BLOCK, &block_signal, NULL);
			printf("%s: ID:%i is in %s state. \t Run time: %f ms \n", table[i].thread_name, i, state_msg, table[i].run_time/1000000);
			sigprocmask (SIG_UNBLOCK, &block_signal, NULL);	
		}
	}
}

/*
* Round robin scheduler. When a thread is preempted it's state will be changed RUNNABLE
* and the preempting thread will be switched to RUNNING. Control block is updated accordingly
*/
void scheduler(){
	scheduler_count++;

	// Exit if the runqueue is empty and current thread is in state EXIT
	if(list_empty(runqueue) && table[running_thread].state==EXIT){
		thread_done=1;
		swapcontext(&table[running_thread].context, &default_context);
	} else if (!list_empty(runqueue)){
		// Make a thread WAIT and switch context with another thread
		int current_thread = running_thread; 
		running_thread = list_shift_int(runqueue);

		if(table[current_thread].state != EXIT && table[current_thread].state!=WAIT){
				runqueue = list_append_int(runqueue, current_thread);
				table[current_thread].state = RUNNABLE;
		}

		table[running_thread].state = RUNNING;
		sigprocmask (SIG_BLOCK, &block_signal, NULL);
		clock_gettime(CLOCK_REALTIME, &table[current_thread].stop_time);
		table[current_thread].run_time += (double) (table[current_thread].stop_time.tv_nsec - table[current_thread].start_time.tv_nsec);
		clock_gettime(CLOCK_REALTIME, &table[current_thread].start_time);
		sigprocmask (SIG_UNBLOCK, &block_signal, NULL);
		swapcontext(&table[current_thread].context, &table[running_thread].context); 
	} 
}
