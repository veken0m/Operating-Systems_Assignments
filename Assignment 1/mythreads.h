
#ifndef __myThreads_H_
#define __myThreads_H_

 #include <ucontext.h>
 #include <slack/std.h>  
 #include <slack/list.h>


  	typedef struct _mythread_control_block{
		ucontext_t context;
		char *thread_name;
		int thread_id;
		int state;
		struct timespec start_time;
		struct timespec stop_time;
		double run_time;
	} mythread_control_block;

	typedef struct _mythread_semaphore{
		int initial_value;
		int in_use;
		int value;
		List *wait_list;
	} mythread_semaphore;

int mythread_init();
int mythread_create(char *threadName, void (*threadfunc)(), int stacksize);
void mythread_exit();
void set_quantum_size(int quantum_size);
void runthreads();
int create_semaphore(int value);
void semaphore_wait(int semaphore);
void semaphore_signal(int semaphore);
void mythread_state();
void scheduler();

#endif
