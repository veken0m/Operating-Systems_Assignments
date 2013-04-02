/*
 * Build: gcc -o pa2_test pa2_test.c mymalloc.c
 * Usage: setarch i686 -R ./pa2_test
 */

/*  
  - Assignment 2 -
  Author: Michael Lagace
  Student ID: 260372922
  Date: March 31, 2013
  Course: Operating Systems - ECSE 427
  Instructor: Prof. Maheswaran
 */

*NOTE* Make sure to run the program with address space randomization
disabled by running as follows: "setarch i686 -R ./pa2_test". 
Running with randomization causes segmentation faults.

Running the test consistently outputs the following:
--------------------------------------------------- 

Hole finding test....
				 Passed

Program break expansion test...
				 Passed
Check for first fit algorithm.... 
				 Passed
Check for best first algorithm.... 
				 Passed
Print some information..

--* Allocation Stats *--
Allocated                	25850880 Bytes 	(~25245 kB)
Free space               	12015680 Bytes 	(~11734 kB)
Largest Contiguous block 	1245184 Bytes 	(~1216 kB)

