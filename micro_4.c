/*
 * Populates a random integer array, writes array to file and allows user 
 * to retrieve an integer by providing it's location 
 *
 * Build: gcc -o micro_4 micro_4.c
 * Usage: ./micro_4 value_to_retreive_int
 * EX:    ./micro_4 6
 */

/*  
  - Micro Assignment 3 -
  Author: Michael Lagace
  Student ID: 260372922
  Date: Jan 15, 2013
  Course: Operating Systems - ECSE 427
  Instructor: Prof. Maheswaran
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv){

	int buffer = 1;
	int value;
        int offset = 4;
        int count = 100;    // int array size
        int integer[count];

        if(argv[1] == NULL){
		printf("Usage: ./micro_4 int_between_0-99 \n");
		return 0;
        }
        // Open file; Create if non-existent
	int file = open("file.bin", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

	sscanf (argv[1],"%d",&value); // Convert arg1 into int
        lseek(file,0,0);              // Seek to beginning of file

        // Randomly populate the integer array
	// and print out array 10 values per line
        // Highlight value retrived with "* *"
        int i;
	printf("Contents of file: \n");
        for(i = 0; i < count; i++){
		srand(value + i);      // rand() seed
        	integer[i] = rand()%100;
		if(i == value){
			printf("*");		
		}
                printf("[%d]", integer[i]);
		if(i == value){
			printf("*");		
		}
                if(i%10 == 9){
			printf("\n");
		}
        }

	write(file,&integer,count*offset); // Write the array to file
	lseek(file,value * offset,0);      // Seek to value we want
	read(file,&buffer, sizeof(buffer));// Read value
        close(file);	
	printf("\nInteger %d is %d\n\n", value, buffer);	
	return 0;
}
