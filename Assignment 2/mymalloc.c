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

#include <stdio.h>      
#include <stdlib.h>  
#include <unistd.h>
#include "mymalloc.h"  

 int selected_policy = 1;            // allocation policy
 int bytes_allocated = 0;            // number of bytes allocated
 const int  buffer_size = 128*1024;  // size of the buffer
 block *head = (const char *)&end;   // head of the free block list

/**
 * Function that finds a free block based on selected policy
 */ 
 void *find_free_block(int size){
    // Make a pointer that points to the head of the free list
    block *ptr = head;
    // First fit, finds first free block that can fit the size we want to allocate
    if(selected_policy == FIRST_FIT)
    {
          while(ptr!=NULL){
            // return free block if big enough
            if(ptr->length >= size) return head;
            // goto next block
            ptr=ptr->next_block;
          }
          // return NULL if free blocks are too small
          return NULL;
      
    // Best fit, finds smallest free block that can fit the the size we want to allocate
    } else { 
          int free_space = 0;
          block *best_block = head;
          // loop through free list
          while(ptr->next_block != NULL){
            if(ptr->length >= size){
              // if found a good fit, update best_block
              if(best_block->length > ptr->length) best_block = ptr;
              // Found some space
              free_space = 1;
            }
            ptr=ptr->next_block;
          }
          if(free_space){ 
            // Found a fit; return block     
            return best_block->previous_block;
          } else {  
            // No fit; return NULL
            return NULL;
          }
    }
 }

 /**
  * Specifies what memory allocation policy to use.
  */
 void my_mallopt(int policy){
      selected_policy = policy;
 }

 /**
  * Allocates memory in the heap. Takes in size in bytes and returns null if 
  * allocation is not successful
  */ 
 void *my_malloc(int size){
      // Check if the head is initialized
      block *ptr_test = head;
      if(ptr_test->buffer == NULL){
           // Create the first block
            *head = (block){0, NULL, NULL, NULL};
            block *ptr = head;
            ptr->length = buffer_size;
            ptr->buffer = (char *)(ptr + 1);
            // set the program break 
            brk(head + (sizeof(block)+buffer_size));  
      }
      
      block *ptr = head;
      // find a free block
      block *current_block = find_free_block(size);
      if(current_block==NULL){
            //go to the end of the free list
            while(ptr->next_block!=NULL){
                ptr=ptr->next_block;
            }
            // while free block is not found
            while(find_free_block(size)==NULL){
                // allocated additional 128 kB
                ptr->length=ptr->length+(buffer_size);
                sbrk(buffer_size);
            }
            // enough space was found, make the block the current block
            current_block=ptr;
      }
      block *new_block;
      if (current_block==head){
            // Update head
            head = (block *) (current_block->buffer + size);
            // Create a new block to be our new head
            *head = (block){0, NULL, NULL, NULL};
            new_block = head;
      } else { // The current block is not the head
            new_block = (block *) (current_block->buffer+size);
      }
      new_block->previous_block = current_block;
      new_block->next_block = current_block->next_block;
      new_block->length = ((current_block->length)-size)-(sizeof(block));
      new_block->buffer = (char *)(new_block +1);
      current_block->length=size;
      current_block->next_block=new_block;
      // current block point to the new free block
      if(current_block->previous_block!=NULL){
        (current_block->previous_block)->next_block=new_block;
      }
      // Increase allocationed size and return buffer
      bytes_allocated+=size;
      return current_block->buffer;
 } 

 /**
  * Deallocates a block of memory. Takes in a pointer to a block previous_blockiously allocated
  * by mymalloc and dellocates it. The deallocated block is then added to the free block
  * list. Also reduces program break if top free block is over 128KB
  */
 void my_free(void *pointer) {
      block *ptr = pointer;
      //Point to the start of the block
      ptr = ptr-1;
      int bytes_freed = ptr->length;

      int left_free=0;
      int right_free=0;
      //Check if left is free
      if(((ptr->previous_block)->previous_block)->next_block!=ptr) {
          left_free = 1;
      }
      // Check if right is free
      if(((ptr->buffer)+ (ptr->length) == (ptr->previous_block)->next_block) && (ptr->next_block!=NULL)) {
          right_free = 1;
      }

      // both sides are free? merge them.
      if(right_free && left_free){
            if((ptr->previous_block)< head){
              head=ptr;
            }
            // update previous and next blocks as well as the size
            (ptr->previous_block)->next_block=(ptr->next_block)->next_block;
            (ptr->previous_block)->length= (ptr->previous_block)->length + ptr->length + (ptr->next_block)->length +2*sizeof(block);
            bytes_freed=(ptr->length);
      } // right side is free? merge.
      else if(right_free && !left_free){
            if(ptr < head){
              head=ptr;
            }
            // update previous and next blocks as well as the size
            ptr->length=(ptr->next_block)->length + ptr->length+ sizeof(block);
            block *right_ptr=ptr->next_block;
            if((right_ptr->next_block)==NULL){
              ptr->next_block=NULL;
            }
            else{
              ptr->next_block=(ptr->next_block)->next_block;
            }
      }
      // left side is free? merge.
      else if(left_free && !right_free){
            if((ptr->previous_block)< head){
                head=ptr;
            }
            // update previous and next blocks as well as the size
            (ptr->previous_block)->next_block = ptr->next_block;
            (ptr->previous_block)->length= (ptr->previous_block)->length + ptr->length+sizeof(block);
      } else {
            //don't merge anything
            if(ptr < head){
              head=ptr;
            }
            // update previous block
            (ptr->previous_block)->next_block=ptr;
      }
      // Remove freed bytes from total bytes allocated
      bytes_allocated-=bytes_freed;
 }

 /**
  * Prints total number of bytes allocated, total free space, 
  * and largest contiguous free space.
  */
  void my_mallinfo(){

        block *ptr = head;
        int largest_cont_free = 0;
        int total_free =0;

        while(ptr!=NULL){
          // sum up all the free space
          total_free+=ptr->length;
          if(ptr->length>largest_cont_free){
            // if larger than previous_blockous largest, replace
            largest_cont_free=ptr->length;
          }
          // next block 
          ptr=ptr->next_block;
        }

      printf("\n--* Allocation Stats *--\n");
      printf("Allocated                \t%d Bytes \t(~%d kB)\n", (bytes_allocated-buffer_size),(bytes_allocated-buffer_size)/1024);
      printf("Free space               \t%d Bytes \t(~%d kB)\n", total_free, total_free/1024);
      printf("Largest Contiguous block \t%d Bytes \t(~%d kB)\n\n", largest_cont_free, largest_cont_free/1024);
    }