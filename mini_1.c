/*
 * Prints out the location of various memory segments of the program:
 * TEXT, BSS, DATA, HEAP, STACK and MMAP
 *
 * Build: gcc -o mini_1 mini_1.c
 * Usage: setarch i686 -R ./mini_1
 */

/*  
  - Mini Assignment 1 -
  Author: Michael Lagace
  Student ID: 260372922
  Date: Feb 19, 2013
  Course: Operating Systems - ECSE 427
  Instructor: Prof. Maheswaran
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int global_uninitialized;    // Stored in BSS
int global_initialized = 10; // Stored in DATA
const int constant = 100;    // Stored in TEXT

extern char etext; //TEXT
extern char edata; //DATA
extern char end;   //BSS

int main(void) {
	static int uninitialized; // Stored in BSS
	static int initialized = 1024; // Stored in DATA
	char *heap = (char*)malloc(initialized);

     label:
	// Print out location of TEXT segment
	printf("\n----------------------TEXT------------------------\n");
	printf("Location of main() is %p\n", main);
	printf("Location of label is %p\n", &&label);
	printf("END of TEXT is %p\n", &etext);
	// Print out location of DATA segment
	printf("----------------------DATA------------------------\n");
	printf("Location of initialized global variable is %p\n", &global_initialized);
	printf("Location of initialized variable is %p\n", &initialized);
	printf("END of DATA is %p\n", &edata);
	// Print out location of BSS segment
	printf("----------------------BSS------------------------\n");
	printf("Location of uninitialized variable is %p\n", &uninitialized);
	printf("Location of uninitialized global variable is %p\n", &global_uninitialized);
	printf("END of BSS is %p\n", &end);
	// Print out location of HEAP segment
	printf("----------------------HEAP------------------------\n");
	printf("Heap starts at %p\n", heap);
	printf("Heap ends at %p\n", sbrk(0));
	// Print out location of MMAP segment
	printf("------------------MEMORY_MAPPING------------------\n");
	int fd = open("file_passed_to_mmap.txt", O_RDONLY);  
  	// Get the size of the file
  	struct stat filestat;
  	if (fstat(fd, &filestat) == -1)
    		perror("Get file stat");
  	// Do a memory map of the file.
  	char *ptr = (char *)mmap(NULL, filestat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	printf("Beginning of mapped file: %p\n", ptr);
	printf("End of mapped file:       %p\n", &ptr[filestat.st_size-1]);
  	// Unmap the memory region.
  	munmap(ptr, filestat.st_size);
	// Print out location of STACK segment
	printf("----------------------STACK-----------------------\n");
	printf("Stack starts at %p\n", alloca(0));
	printf("Stack ends at %p\n", alloca(1024));
	
	
  return 0;
}
