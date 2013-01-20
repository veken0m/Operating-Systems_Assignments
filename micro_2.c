/*
 * Presents the user with a tiny shell that 
 * excecutes commands in a child procress
 *
 * Build: gcc -o micro_2 micro_2.c
 * Usage: ./micro_2
 */

/* 
  - Micro Assignment 2 -
  Author: Michael Lagace
  Student ID: 260372922
  Date: Jan 10, 2013
  Course: Operating Systems - ECSE 427
  Instructor: Prof. Maheswaran
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(void) {

  pid_t childPid;
  int count;
  char  input[16];
  char  *arguments[16];
  
  while (42) {
  printf("Command Prompt > ");
  gets(input);
  count = 0;

  /* tokenize command */
  arguments[count] = strtok(input," ");
  while (arguments[0] != NULL && count < 15) {
    count++;
    arguments[count] = strtok(NULL," ");
  }
   
    switch (childPid = fork()){
      case -1: /*Fork failed*/
        printf("ERROR: forking child process failed\n");
 
      case 0: /*Child Process*/
        /* execute the command  */
        if (execvp(arguments[0], arguments) < 0) {
           /* Output error message if execvp() fails */
           printf("ERROR: Unknown command\n");
           exit(1);
         }
     
      default: /*Parent Process*/
         wait(); //Wait for child process to finish
     } 
  }

  return 0;
}

