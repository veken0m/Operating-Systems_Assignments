/*
 * Build: gcc -o sfs_test sfs_test.c sfs_api.c
 * Usage: ./sfs_test
 */

/*  
  - Assignment 3 -
  Author: Michael Lagace
  Student ID: 260372922
  Date: April 08, 2013
  Course: Operating Systems - ECSE 427
  Instructor: Prof. Maheswaran
 */


Output of running sfs_test on my sfs_api results in the following output:
-------------------------------------------------------------------------

michael@ubuntu:~/Desktop$ ./sfs_test
Two files created with zero length:
----------Root directory file list----------
CDTNDDWT.FZI	0 Bytes
WSSQARQA.HVP	0 Bytes

File CDTNDDWT.FZI now has length 16334 and WSSQARQA.HVP now has length 18066:
----------Root directory file list----------
CDTNDDWT.FZI	16334 Bytes
WSSQARQA.HVP	18066 Bytes
Created 100 files in the root directory
Simultaneously opened 100 files
Trying to fill up the disk with repeated writes to BYKOXHAT.LQK.
(This may take a while).
Write failed after 15940 iterations.
If the emulated disk contains just over 16322560 bytes, this is OK
Test program exiting with 0 errors

-------------------------------------------------------------------------

The resulting virtual drive file "sfs_virtual_drive" is approx 16,777,216 bytes (16.8MB)


