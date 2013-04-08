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

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include "disk_emu.h"  
#include "sfs_api.h"  

root_directory_table RDT[MAX_FILES+1]; 
file_allocation_table FAT[BLOCKS]; 
file_descriptor_table FDT[MAX_FILES];

int fat_index_temp, fileID_temp = 0;
int used_blocks = RESERVED_BLOCKS;

/*
 * Formats the file system, creates necessary disk resident data,
 * structures and initializes them
 *
 * - Fresh is a flag to signal that the file system should be made form scratch
 * - If fresh is false file system is opened from disk (assuming a valid one exists) 
 */
void mksfs(int fresh){
	
	// Make a new disk
	if(fresh){
		init_fresh_disk(SFS_filename,BLOCK_SIZE, BLOCKS);
		init_disk(SFS_filename, BLOCK_SIZE, BLOCKS);

		int i;
		for(i=0;i<MAX_FILES;i++){
			RDT[i].exists = END_OF_FILE;
			RDT[i].fat_index = END_OF_FILE;
		}
		for(i=0;i<BLOCKS;i++){
			FAT[i].data_block_height = END_OF_FILE;
			FAT[i].next = END_OF_FILE;
		}
		write_blocks(0,1,&RDT);
		write_blocks(1,1,&FAT);
		write_blocks(BLOCKS-1,1,&used_blocks);
	// Open previously made disk
	} else {
		init_disk(SFS_filename,BLOCK_SIZE, BLOCKS);
		// Load tables
		read_blocks(0,1, &RDT);
		read_blocks(1,1, &FAT);
		read_blocks(BLOCKS-1,1, &used_blocks);
	}

	// Set the make sure all files are closed and set r/w pointers to correct location
	int j;
	for(j=0;j<MAX_FILES;j++){
		FDT[j].open = 0;
		FDT[j].read = 0;
		FDT[j].write = RDT[j].size;
	}

}

/*
 * Lists the contents of the root directory
 */
void sfs_ls(){
	int i;
	printf("----------Root directory file list----------\n");
	for(i=0;i<MAX_FILES;i++){
		if(RDT[i].exists != END_OF_FILE){
			printf("%s\t%i Bytes\n", RDT[i].file_name, RDT[i].size);
		} 
	}
}

/*
 * Opens a file and returns the index on the file descriptor table. 
 * If does not exist create a new file and set size to 0
 * If exists open the file in append mode
 */
int sfs_open(char *name){
	
	int i;
	int fileID = -1;
	// Get the file ID
	for(i=0;i<MAX_FILES;i++){
		if(RDT[i].exists == 1 && !strcmp(RDT[i].file_name,name)){
				fileID = i;
		}
	}

	// File does not exist
	if(fileID == -1){
		// Create a new file
		fileID = fileID_temp++; // Assign the next ID
		strcpy(RDT[fileID].file_name,name);
		RDT[fileID].exists = 1;
		RDT[fileID].size = 0;
		RDT[fileID].fat_index = fat_index_temp++; // Increment FAT index
		FAT[RDT[fileID].fat_index].data_block_height = END_OF_FILE;
		FAT[RDT[fileID].fat_index].next = END_OF_FILE;
	    write_blocks(0,1,&RDT);
		write_blocks(1,1,&FAT);
		write_blocks(BLOCKS-1,1,&used_blocks);
	}

	// Open the file and set the read/write pointers
	FDT[fileID].open = 1;
	FDT[fileID].read = 0;
	FDT[fileID].write = RDT[fileID].size;
	return fileID;
}

/*
 * Closes a file and removes the entry from the open file descriptor table
 */
int sfs_close(int fileID){ 
	// If file open close it
	if(FDT[fileID].open == 1){
		FDT[fileID].open = 0;
		FDT[fileID].read = 0;
		FDT[fileID].write = 0;
		return 0;
	} else{
		// File is not open...	
		return 1;
	}
}

/*
 * Writes the "length" bytes of the buffered data in "buf" onto the open file,
 * starting from the current pointer. This will result the size of the file to increase by "length" bytes
 */
int sfs_write(int fileID, char *buf, int length){

	// Check if the file is open
	if(FDT[fileID].open == 0){
		return -1;
	}

	// If EOF, give the file a new block
	int FAT_entry = RDT[fileID].fat_index;
	if(FAT[FAT_entry].data_block_height==END_OF_FILE){
		FAT[FAT_entry].data_block_height=used_blocks++;
		FAT[FAT_entry].next=END_OF_FILE;
	}
	
	// Point to the end of the file in order to write
	int wPtr = FDT[fileID].write; 
	int wPtrBlk = (wPtr - wPtr%BLOCK_SIZE)/BLOCK_SIZE; 
	
	//circulate through to the write block
	int error = 0;
	int lastWriteSize  = 0;
	int blkCounter = 0;
	while(blkCounter < wPtrBlk){
		FAT_entry = FAT[FAT_entry].next;
		blkCounter++;
	}
	
	// Find the size of the first write
	int bytesWritten = min(BLOCK_SIZE - wPtr%BLOCK_SIZE,length); 
	char firstBlk[BLOCK_SIZE];	
	read_blocks(FAT[FAT_entry].data_block_height, 1, &firstBlk);
	memcpy(firstBlk + wPtr%BLOCK_SIZE, buf, bytesWritten);
	write_blocks(FAT[FAT_entry].data_block_height, 1, &firstBlk);
	
	// Can write full block
	while((length-bytesWritten)>=BLOCK_SIZE){
		int nextFatEntry = FAT[FAT_entry].next;
		// EOF? Append a new block to file
		if(nextFatEntry == END_OF_FILE){
			FAT[FAT_entry].next = fat_index_temp++;
			nextFatEntry = FAT[FAT_entry].next;
			FAT[nextFatEntry].data_block_height=used_blocks++;
		}
		if(FAT[nextFatEntry].data_block_height >= BLOCKS-RESERVED_BLOCKS){
			goto update;
		}
		// Write whole block
		write_blocks(FAT[nextFatEntry].data_block_height, 1, buf+bytesWritten);
		bytesWritten+=BLOCK_SIZE;		
		FAT_entry = nextFatEntry;
	}

	// Less than a block to write
	if(bytesWritten <= length){ 
		int nextFatEntry = FAT[FAT_entry].next;
		lastWriteSize = length - bytesWritten;
		// EOF, assign a new block
		if(nextFatEntry==END_OF_FILE) {		
			FAT[FAT_entry].next = fat_index_temp++;
			nextFatEntry = FAT[FAT_entry].next;
			FAT[nextFatEntry].data_block_height=used_blocks++;
			if(FAT[nextFatEntry].data_block_height >= BLOCKS-RESERVED_BLOCKS){
				goto update;
			}
			error = write_blocks(FAT[nextFatEntry].data_block_height, 1, buf+bytesWritten);
		// Block available, write to it
		} else { 
			char newData[BLOCK_SIZE];			
			read_blocks(FAT[nextFatEntry].data_block_height, 1, &newData);
			memcpy(&newData, buf+bytesWritten, lastWriteSize);
			if(FAT[nextFatEntry].data_block_height >= BLOCKS-RESERVED_BLOCKS){
				goto update;
			}
			error = write_blocks(FAT[nextFatEntry].data_block_height, 1, newData);
		}
		update:
		bytesWritten+=lastWriteSize;
	}

	RDT[fileID].size=max(RDT[fileID].size, wPtr+length);
	FDT[fileID].write+=length;
	
	// Update disk
	write_blocks(0,1,&RDT);
	write_blocks(1,1,&FAT);
	write_blocks(BLOCKS-1,1,&used_blocks);

	// Return bytes written or if errors, return number of errors
	if(error > 0){
		return bytesWritten;
	} else {
		return error;
	}
}

/*
 * Reads the "length" bytes from the file and writes them into the buffer "buf",
 * starting from the current beginning of the file. Does not read beyond EOF.
 */
int sfs_read(int fileID,char *buf, int length){

	// Check if file is open
	if(FDT[fileID].open == 0){
		return -1;
	}

	// If trying to read more than written to disk, limit read length
	int adjustedLength = 0;
	if(FDT[fileID].write < length){
		adjustedLength = FDT[fileID].write;
	} else {
		adjustedLength = length;
	}
	
	// Set the read pointer
	int rPtr = FDT[fileID].read;
	int rPtrBlock = (rPtr - rPtr%BLOCK_SIZE)/BLOCK_SIZE;
	int currentFatEntry = RDT[fileID].fat_index;
	
	int i;
	for(i = 0; i < rPtrBlock; i++){
		currentFatEntry = FAT[currentFatEntry].next;
	}

	char tempBuffer[BLOCK_SIZE];
	read_blocks(FAT[currentFatEntry].data_block_height, 1,tempBuffer);
	int bytesRead = min(RDT[fileID].size, min(length, (BLOCK_SIZE - rPtr%BLOCK_SIZE)));
	memcpy(buf,tempBuffer + rPtr%BLOCK_SIZE, bytesRead); 	

	// Read a whole block
	while(adjustedLength - bytesRead >= BLOCK_SIZE && currentFatEntry!=END_OF_FILE){
		currentFatEntry = FAT[currentFatEntry].next;
		read_blocks(FAT[currentFatEntry].data_block_height, 1,buf+bytesRead);
		bytesRead+=BLOCK_SIZE;
	}
	// Read less than a block
	if(bytesRead<adjustedLength){
		read_blocks(FAT[FAT[currentFatEntry].data_block_height].next, 1,(void*) &tempBuffer);
		memcpy(buf+bytesRead, &tempBuffer, adjustedLength - bytesRead);
		bytesRead+=(adjustedLength- bytesRead);
	}
	// update read pointer
	FDT[fileID].read+=bytesRead;
	return bytesRead;
}