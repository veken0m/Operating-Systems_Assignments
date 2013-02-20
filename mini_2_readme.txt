/*  
  - Benchmarks for mini_2.c -
  Author: Michael Lagace
  Student ID: 260372922
  Date: Feb 19, 2013
  Course: Operating Systems - ECSE 427
  Instructor: Prof. Maheswaran
 */


SYNCED, mutex and condition variables
N = 100000

0.183550 seconds
0.193863 seconds
0.204924 seconds
0.183966 seconds
0.179747 seconds

AVG: 0.1892088 seconds 

UNSYNCED, no mutex and no condition variables
N = 100000

0.019483 seconds
0.015186 seconds
0.016354 seconds
0.019930 seconds
0.015935 seconds

AVG: 0.0173776 seconds

Sync. overhead = 0.1892088 - 0.0173776 = 0.1718312 seconds

0.1718312/0.0173776 = 988% overhead! (for this particular example)

Even though there may be significant overhead, it is critical to syncronize threads in order to prevent race conditions and deadlock
