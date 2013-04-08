/*
 * Build: gcc -o sfs_test sfs_test.c sfs_api.c
 * Usage: ./sfs_test
 */

/*  
  - Assignment 3 -
  Author: Michael Lagace
  Student ID: 260372922
  Date: March 31, 2013
  Course: Operating Systems - ECSE 427
  Instructor: Prof. Maheswaran
 */

#ifndef SFS_API_H
#define SFS_API_H

#define BLOCK_SIZE 2048
#define RESERVED_BLOCKS 2
#define SFS_filename "sfs_virtual_drive"
#define BLOCKS 8192
#define MAX_FILES 512
#define FILENAME_LENGTH 12
#define END_OF_FILE -1

// Min and Max funtion defines
#define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )

// Struct for the Root Directory Table
typedef struct {
    int size; // size of the file
    int exists; // file exists 
 	int fat_index; // FAT index
 	char file_name[FILENAME_LENGTH]; // File name
 	char dummy_bytes[2]; // dummy bytes needed for alignment
} root_directory_table;

// Struct for the File Descriptor Table
typedef struct {
 	int open; // file open or closed
 	int read; // read pointer
 	int write; // write pointer
} file_descriptor_table;

// Struct for the File Allocation Table
typedef struct {
 	int data_block_height; // Block at that FAT index
 	int next;			   // Next Block
} file_allocation_table;

void mksfs(int fresh);
void sfs_ls(void);
int sfs_open(char *name);
int sfs_close(int fileID);
int sfs_write(int fileID, char *buf, int length);
int sfs_read(int fileID, char *buf, int length);

#endif