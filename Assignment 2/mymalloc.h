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

#define FIRST_FIT 1
 #define BEST_FIT  2

 extern char end;

/**
 * Struct for block of free memory
 *
 * - Contains pointer to the next and previous free block of memory
 * - It also has the size in bytes and a pointer to the buffer to store data 
 */
typedef struct block {
 	int length;
    struct block *previous_block;
 	struct block *next_block;
 	unsigned char *buffer;
} block;

 extern void *my_malloc(int size);
 extern	char *my_malloc_error;
 extern void my_free(void *ptr);
 extern void my_mallopt(int policy);
 extern void my_mallinfo();
