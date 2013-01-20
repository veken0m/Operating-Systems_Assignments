/*
 * Parses linux /proc/pid/status and prints out:
 *
 * PID, PPID, Name, State, Stack Usage, 
 * voluntary_ctxt_switches, nonvoluntary_ctxt_switches,
 * and environment variables
 *
 * Build: gcc -o micro_1 micro_1.c
 * Usage: ./micro_1
 */

/* 
  - Micro Assignment 1 -
  Author: Michael Lagace
  Student ID: 260372922
  Date: Jan 15, 2013
  Course: Operating Systems - ECSE 427
  Instructor: Prof. Maheswaran
 */

#include <stdio.h>
#include <unistd.h>

int main(void) {

  FILE *input = NULL;
  extern char **environ;
  char name[20];	// %s - Name   
  char state;		// %c - State 
  char state_full[20];  // %s - State
  char stack_usage[20];	// %s - VmStk
  int vcs;		// %d - voluntary_ctxt_switches
  int ncs;		// %d - nonvoluntary_ctxt_switches
  char buff[50];        // string for '/proc/{PID}/status'
  char buf[50];         // buffer that holds a single line of status file

  sprintf(buff, "/proc/%d/status", getpid()); 
  input = fopen(buff, "r");
  
  // Loop through every line of the status file
  while(fgets(buf,50,input)){
    
    // Parse values of interest
    sscanf(buf,"Name:\t%s",name); 
    sscanf(buf,"State:\t%c %s",&state, state_full);   
    sscanf(buf,"VmStk:\t%s",stack_usage);    
    sscanf(buf,"voluntary_ctxt_switches: %d",&vcs);    
    sscanf(buf, "nonvoluntary_ctxt_switches:\t%d", &ncs);  
  } 

    // Print out values of interest
    printf("\n/proc/{PID}/status:\n\n");
    printf("Process ID is %d\n",getpid()); 
    printf("Parent process ID is %d\n",getppid());
    printf("Name of process is %s\n", name);  
    printf("State of process is %c %s\n", state, state_full);   
    printf("Stack usage of process is %s kB\n", stack_usage);  
    printf("%d voluntary context switches\n", vcs);  
    printf("%d nonvoluntary context switches\n\n", ncs);
    
    // Interate through environ and print environment variables
    printf("Environment Variables:\n\n");
    int i = 0;
    while(environ[i]) {
      printf("%s\n", environ[i++]);
    }

  return 0;
}


