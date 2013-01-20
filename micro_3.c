/* 
 * Uses a pipeline to pass output of "ls" into input of "wc -l"
 * Each command is is executed in a different process
 *
 * Build: gcc -o micro_3 micro_3.c
 * Usage: ./micro_3
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
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
  
  char *ls[2] = {"ls",NULL};
  char *wc[3] = {"wc","-l",NULL};
  pid_t childPid;
  int count;
  const int BUF_SIZE = 256;
  char  input[16];
  int pfd[2];
  char buf[BUF_SIZE];
  ssize_t numRead;

  if(pipe(pfd) == -1){
  	printf("ERROR: pipe failed\n");
  }
   
  switch (childPid = fork()){
      case -1: /*Fork failed*/
        printf("ERROR: forking child process failed\n");
     
      case 0: /*Child Process*/
        dup2(pfd[1], 1);
        close(pfd[0]);
        if (execvp(ls[0], ls) < 0){
        	printf("ERROR: Unknown command (child)\n");
        	exit(1);
	} else {
        exit(0); 
	}
      default: /*Parent Process*/
        dup2(pfd[0],0);
        close(pfd[1]);
        if (execvp(wc[0], wc) < 0){
           printf("ERROR: Unknown command (parent)\n");
           exit(1);
	} else {
        exit(0);
        }
   }

  return 0;
}
