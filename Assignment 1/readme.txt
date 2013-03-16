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


michael@ubuntu:~/Desktop$ ./my-test
thread 0: ID:0 is in RUNNABLE state. 	 Run time: 0.000000 ms 
thread 1: ID:1 is in RUNNABLE state. 	 Run time: 0.000000 ms 
thread 2: ID:2 is in RUNNABLE state. 	 Run time: 0.000000 ms 
thread 3: ID:3 is in RUNNABLE state. 	 Run time: 0.000000 ms 
thread 4: ID:4 is in RUNNABLE state. 	 Run time: 0.000000 ms 
thread 5: ID:5 is in RUNNABLE state. 	 Run time: 0.000000 ms 
thread 6: ID:6 is in RUNNABLE state. 	 Run time: 0.000000 ms 
thread 7: ID:7 is in RUNNABLE state. 	 Run time: 0.000000 ms 
thread 8: ID:8 is in RUNNABLE state. 	 Run time: 0.000000 ms 
thread 9: ID:9 is in RUNNABLE state. 	 Run time: 0.000000 ms 
thread 0: ID:0 is in EXIT state. 	 Run time: 5.808042 ms 
thread 1: ID:1 is in EXIT state. 	 Run time: 6.305864 ms 
thread 2: ID:2 is in EXIT state. 	 Run time: 6.798637 ms 
thread 3: ID:3 is in EXIT state. 	 Run time: 7.286292 ms 
thread 4: ID:4 is in EXIT state. 	 Run time: 7.802158 ms 
thread 5: ID:5 is in EXIT state. 	 Run time: 8.322983 ms 
thread 6: ID:6 is in EXIT state. 	 Run time: 8.813335 ms 
thread 7: ID:7 is in EXIT state. 	 Run time: 9.286434 ms 
thread 8: ID:8 is in EXIT state. 	 Run time: 9.776703 ms 
thread 9: ID:9 is in EXIT state. 	 Run time: 5.309786 ms 
The counter is 50
The result is 151402.656521
>>> Thread library PASSED the Test 1

michael@ubuntu:~/Desktop$ ./my-test
thread 0: ID:0 is in RUNNABLE state. 	 Run time: 0.000000 ms 
thread 1: ID:1 is in RUNNABLE state. 	 Run time: 0.000000 ms 
thread 2: ID:2 is in RUNNABLE state. 	 Run time: 0.000000 ms 
thread 3: ID:3 is in RUNNABLE state. 	 Run time: 0.000000 ms 
thread 4: ID:4 is in RUNNABLE state. 	 Run time: 0.000000 ms 
thread 5: ID:5 is in RUNNABLE state. 	 Run time: 0.000000 ms 
thread 6: ID:6 is in RUNNABLE state. 	 Run time: 0.000000 ms 
thread 7: ID:7 is in RUNNABLE state. 	 Run time: 0.000000 ms 
thread 8: ID:8 is in RUNNABLE state. 	 Run time: 0.000000 ms 
thread 9: ID:9 is in RUNNABLE state. 	 Run time: 0.000000 ms 
thread 0: ID:0 is in EXIT state. 	 Run time: 64.173094 ms 
thread 1: ID:1 is in EXIT state. 	 Run time: 65.193010 ms 
thread 2: ID:2 is in EXIT state. 	 Run time: 66.190128 ms 
thread 3: ID:3 is in EXIT state. 	 Run time: 67.172188 ms 
thread 4: ID:4 is in EXIT state. 	 Run time: 68.205006 ms 
thread 5: ID:5 is in EXIT state. 	 Run time: 69.175493 ms 
thread 6: ID:6 is in EXIT state. 	 Run time: 70.201102 ms 
thread 7: ID:7 is in EXIT state. 	 Run time: 71.298111 ms 
thread 8: ID:8 is in EXIT state. 	 Run time: 72.192380 ms 
thread 9: ID:9 is in EXIT state. 	 Run time: 63.183188 ms 
The counter is 50
The result is 151402.656521
>>> Thread library PASSED the Test 1


