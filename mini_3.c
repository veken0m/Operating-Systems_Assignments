/*
 * Simulation of Banker's Algorithm
 *
 * Attempts to assign resources while checking if will put other processes in unsafe state
 * Once assigning resources to any process is unsafe, prints out resource allocation matrix
 *
 * Build: gcc -o mini_3 mini_3.c -lm
 * Usage: ./mini_3
 */

/*  
  - Mini Assignment 3 -
  Author: Michael Lagace
  Student ID: 260372922
  Date: Feb 19, 2013
  Course: Operating Systems - ECSE 427
  Instructor: Prof. Maheswaran
 */


#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>


int main(int argc, char *argv[]) {
	// seed rand()
	srand(time(NULL)+getpid());

	// randomly pick between 5-10 processes
	int N = (rand()%5)+6;
	printf("\nNumber of processes: %i\n", N);

	// Resources needed by each process to finish	
	int resourceA[N];
	int resourceB[N];
	int resourceC[N];
	int resourceD[N];

	// Resources current used by each process
	int allocatedResourceA[N];
	int allocatedResourceB[N];
	int allocatedResourceC[N];
	int allocatedResourceD[N];

	// Resources required by each process to finish
	int requiredResourceA[N];
	int requiredResourceB[N];
	int requiredResourceC[N];
	int requiredResourceD[N];	

	// Max available resources
	int resourceA_max = 0;
	int resourceB_max = 0;
	int resourceC_max = 0;
	int resourceD_max = 0;

	// Currently available resources
	int resourceA_avail = 0;
	int resourceB_avail = 0;
	int resourceC_avail = 0;
	int resourceD_avail = 0;

	int unsafe_array[N];
	int process_done[N];

	int i = 0;
	printf("Resource format: [A,B,C,D]\n\n");
	for(i = 0;N > i; i++){
		// Initialize all the arrays
		allocatedResourceA[i] = 0;
		allocatedResourceB[i] = 0;
		allocatedResourceC[i] = 0;
		allocatedResourceD[i] = 0;
		process_done[i] = 0;
		unsafe_array[i] = 0;
	
		// Randomly pick required resources for each process
		resourceA[i] = (rand()%8)+5;
		resourceB[i] = (rand()%8)+5;
		resourceC[i] = (rand()%8)+5;
		resourceD[i] = (rand()%8)+5;
		requiredResourceA[i]=resourceA[i];
		requiredResourceB[i]=resourceB[i];		
		requiredResourceC[i]=resourceC[i];
		requiredResourceD[i]=resourceD[i];
		printf("Process %i needs: [%i,%i,%i,%i]\n", i, resourceA[i], resourceB[i], resourceC[i], resourceD[i]);

		resourceA_max += resourceA[i];
		resourceB_max += resourceB[i];
		resourceC_max += resourceC[i];
		resourceD_max += resourceD[i];
	}

	// Calculate max for each resource;  max = sum_of_needs*0.6
	resourceA_max = ceilf((float)resourceA_max*0.6);
	resourceA_avail = resourceA_max;
        printf("Resource A initial reserve: %i\n", resourceA_max);
	resourceB_max = ceilf((float)resourceB_max*0.6);
	resourceB_avail = resourceB_max;
	printf("Resource B initial reserve: %i\n", resourceB_max);
	resourceC_max = ceilf((float)resourceC_max*0.6);
	resourceC_avail = resourceC_max;
	printf("Resource C initial reserve: %i\n", resourceC_max);
	resourceD_max = ceilf((float)resourceD_max*0.6);
	resourceD_avail = resourceD_max;
	printf("Resource D initial reserve: %i\n", resourceD_max);
	
	int unsafe_count = 0;
	// run while all processes left to run are safe or resources are still available 
	while((unsafe_count  != N) || (resourceA_avail!=0&&resourceB_avail!=0&&resourceC_avail!=0&&resourceD_avail!=0)){
		for(i = 0;N > i; i++){
			unsafe_count = 0;

			// random request for resources between 0-max_available
			int needResourceA = (rand()%resourceA_max);
			int needResourceB = (rand()%resourceB_max);
			int needResourceC = (rand()%resourceC_max);
			int needResourceD = (rand()%resourceD_max);

			// Check if process asking less or equal than it needs; ignore if unsafe or complete
			if(((needResourceA + allocatedResourceA[i]) <= resourceA[i]) 
				&& ((needResourceB + allocatedResourceB[i]) <= resourceB[i]) 
				&& ((needResourceC + allocatedResourceC[i]) <= resourceC[i]) 
				&& ((needResourceD + allocatedResourceD[i]) <= resourceD[i])
				&& !process_done[i] && !unsafe_array[i]){
				
				// Check if process asking less or equal than available
				if((needResourceA <= resourceA_avail && needResourceB <= resourceB_avail 
					&& needResourceC <= resourceC_avail && needResourceD <= resourceD_avail)){
					
					// Check if giving the resources cause unsafe state
					int j = 0;
					for(j = 0;N > j; j++){
						if(((requiredResourceA[j] >= (resourceA_avail - needResourceA)) &&
							(requiredResourceB[j] >= (resourceB_avail - needResourceB)) &&	
							(requiredResourceC[j] >= (resourceC_avail - needResourceC)) &&
							(requiredResourceD[j] >= (resourceD_avail - needResourceD))) && (j!=i)){
							// unsafe!
							unsafe_array[i] = 1;
						}
					}
					// If safe allocate the resources
					if(!unsafe_array[i]){
						// Remove from available resources
						resourceA_avail -= needResourceA;
						resourceB_avail -= needResourceB;
						resourceC_avail -= needResourceC;
						resourceD_avail -= needResourceD;
						// Give the process the resources
						allocatedResourceA[i] += needResourceA;
						allocatedResourceB[i] += needResourceB;
						allocatedResourceC[i] += needResourceC;
						allocatedResourceD[i] += needResourceD;
						// Reduce process required resources to complete
						requiredResourceA[i] -= needResourceA;
						requiredResourceB[i] -= needResourceB;		
						requiredResourceC[i] -= needResourceC;
						requiredResourceD[i] -= needResourceD;

						// If process has all resource it needs, complete and give back
						if(allocatedResourceA[i]==resourceA[i] &&
							allocatedResourceB[i]==resourceB[i] &&
							allocatedResourceC[i]==resourceC[i] &&
							allocatedResourceD[i]==resourceD[i]){
						printf("Process %i complete ... giving back resources\n", i);
						// Release resources
						resourceA_avail += resourceA[i];
						resourceB_avail += resourceB[i];
						resourceC_avail =+ resourceC[i];
						resourceD_avail =+ resourceD[i];
						allocatedResourceA[i] -= resourceA[i];
						allocatedResourceB[i] -= resourceB[i];
						allocatedResourceC[i] -= resourceC[i];
						allocatedResourceD[i] -= resourceD[i];
						// Process complete
					 	process_done[i] = 1;
						}

					} else {
						printf("unsafe ... denied!\n");
					}	
				} 
			}
		}
		// Check how many processes are unsafe or completed		
		int k;
		for(k = 0;N > k; k++){
		  unsafe_count += unsafe_array[k];
		  unsafe_count += process_done[k];
		}			
	}

	// Calculate total allocated resources and print out avail/used resources array
	int totalAlloc[4];
	totalAlloc[0] = resourceA_max - resourceA_avail;
	totalAlloc[1] = resourceB_max - resourceB_avail;
	totalAlloc[2] = resourceC_max - resourceC_avail;
	totalAlloc[3] = resourceD_max - resourceD_avail;
	printf("Total allocation: [%i,%i,%i,%i]\n", totalAlloc[0],totalAlloc[1],totalAlloc[2],totalAlloc[3]);
	printf("Total available: [%i,%i,%i,%i]\n", resourceA_avail,resourceB_avail,resourceC_avail,resourceD_avail);

	// Print allocation matrix for each process
	for(i = 0;N > i; i++){
		printf("Process %i: [%i,%i,%i,%i] \tcompleted: %i\n", i, 
			allocatedResourceA[i],
			allocatedResourceB[i],
			allocatedResourceC[i],
			allocatedResourceD[i], 
			process_done[i]);
	}

	exit(0);
}
