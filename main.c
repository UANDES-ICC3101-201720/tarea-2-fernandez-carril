/*
Main program for the virtual memory project.
Make all of your modifications to this file.
You may add or rearrange any code or data as you need.
The header files page_table.h and disk.h explain
how to use the page table and disk interfaces.
*/

#include "page_table.h"
#include "disk.h"
#include "program.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
struct disk *disk;
char *physmem;
char *virtmem;
int npages;
int nframes;
char *swaptype;
const char *program;
int *frame_age; //saves the age of the frame in use for the key
int *frame_use; //saves the page number, the key is the frame being used
int init_frames = 0;
int page_fault = 0;
int num_disk_write = 0;
int num_disk_read = 0;

void print_frames (){
	for (int i = 0 ; i<nframes; i++){
		if(frame_use[i] != NULL)
			printf("frame:%d, age:%d\n", frame_use[i], frame_age[i]);
	}
}

int key_frame_oldest ()
{
	int max = frame_age[0];
	int key_of_max = 0;
	for(int i = 0 ; i<nframes; i++){
		if(frame_age[i] > max){
			max = frame_age[i];
			key_of_max = i;
		}
	}
	return key_of_max;
}
int fill_mem(){
	for(int i = 0 ; i<nframes; i++){
		if(frame_use[i] == NULL){
			return i;
		}
	}
	return -1;
}
int frame_in_use (int frame){
	for(int i = 0 ; i<nframes; i++){
		if(frame_use[i] != NULL)
			return 1;
	}
	return -1;
}

void replace_frame (int key_of_frame, int page_num){
	frame_use[key_of_frame] = page_num;
	frame_age[key_of_frame] = 0;
}
void age_frames (){
	for(int i = 0 ; i<nframes; i++){
		if(frame_use[i] != NULL)
			frame_age[i]++;
	}
}

void page_fault_handler( struct page_table *pt, int page )
{
	printf("page fault on page #%d\n",page);
	exit(1);
}

void fifo_handler( struct page_table *pt, int page )
{
	int target;
	page_fault++;
	if(init_frames < nframes){
		target = fill_mem();
		frame_use[target] = page;
		frame_age[target] = 0;
		disk_write(disk, page, &physmem[target*PAGE_SIZE]);
		num_disk_write++;
		page_table_set_entry(pt, page, target, PROT_READ|PROT_WRITE|PROT_EXEC);
		disk_read(disk, page, &physmem[target*PAGE_SIZE]);
		num_disk_read++;
		init_frames++;
		age_frames();
	}
	else{
		target = key_frame_oldest();
		disk_write(disk, page, &physmem[target*PAGE_SIZE]);
		num_disk_write++;		
		disk_read(disk, page, &physmem[target*PAGE_SIZE]);
		num_disk_read++;
		page_table_set_entry(pt, page, target, PROT_READ|PROT_WRITE|PROT_EXEC);
		page_table_set_entry(pt, frame_use[target], target, 0);
		replace_frame(target, page);
		init_frames++;
		age_frames();
	}
	//page_table_print(pt);
}

void random_handler( struct page_table *pt, int page )
{
	int target;
	page_fault++;
	if(init_frames < nframes){
		target = fill_mem();
		frame_use[target] = page;
		disk_write(disk, page, &physmem[target*PAGE_SIZE]);
		num_disk_write++;
		page_table_set_entry(pt, page, target, PROT_READ|PROT_WRITE|PROT_EXEC);
		disk_read(disk, page, &physmem[target*PAGE_SIZE]);
		num_disk_read++;
		init_frames++;
	}
	else{
		target = lrand48()%nframes;
		disk_write(disk, page, &physmem[target*PAGE_SIZE]);	
		num_disk_write++;	
		disk_read(disk, page, &physmem[target*PAGE_SIZE]);
		num_disk_read++;
		page_table_set_entry(pt, page, target, PROT_READ|PROT_WRITE|PROT_EXEC);
		page_table_set_entry(pt, frame_use[target], target, 0);
		replace_frame(target, page);	
	}
	//page_table_print(pt);
}

void custom_handler( struct page_table *pt, int page )
{
	int target;
	page_fault++;
	if(init_frames < nframes){
		target = fill_mem();
		frame_use[target] = page;
		disk_write(disk, page, &physmem[target*PAGE_SIZE]);
		num_disk_write++;
		page_table_set_entry(pt, page, target, PROT_READ|PROT_WRITE|PROT_EXEC);
		disk_read(disk, page, &physmem[target*PAGE_SIZE]);
		num_disk_read++;
		init_frames++;
	}
	else{
		target = page_fault%nframes;
		disk_write(disk, page, &physmem[target*PAGE_SIZE]);	
		num_disk_write++;	
		disk_read(disk, page, &physmem[target*PAGE_SIZE]);
		num_disk_read++;
		page_table_set_entry(pt, page, target, PROT_READ|PROT_WRITE|PROT_EXEC);
		page_table_set_entry(pt, frame_use[target], target, 0);
		replace_frame(target, page);
		init_frames++;
		age_frames();
	}
}


int main( int argc, char *argv[] )
{
	int opt = 0;
	while ((opt = getopt(argc, argv, "n:f:a:p:")) != -1)
	{
		switch (opt) {
			case 'n':
				npages = atoi(optarg);
			case 'f':
				nframes = atoi(optarg);
			case 'a':
				swaptype = optarg;
			case 'p':
				program = optarg;

		}
	}

	frame_age = malloc(sizeof(int) * nframes);
	frame_use = malloc(sizeof(int) * nframes);

	disk = disk_open("myvirtualdisk",npages);
	if(!disk) {
		fprintf(stderr,"couldn't create virtual disk: %s\n",strerror(errno));
		return 1;
	}


	struct page_table *pt;

	if (strcmp(swaptype, "fifo") == 0)
	{
		pt = page_table_create(npages, nframes, fifo_handler);
	}
	else if (strcmp(swaptype, "rand") == 0)
	{
		pt = page_table_create(npages, nframes, random_handler);
	}
	else if (strcmp(swaptype, "custom") == 0)
	{
		pt = page_table_create(npages, nframes, random_handler);
	}
	else
	{
		pt = page_table_create(npages, nframes, page_fault_handler);
	}

	virtmem = page_table_get_virtmem(pt);

	physmem = page_table_get_physmem(pt);

	if(!strcmp(program,"sort")) {
		sort_program(virtmem,npages*PAGE_SIZE);

	} else if(!strcmp(program,"scan")) {
		scan_program(virtmem,npages*PAGE_SIZE);

	} else if(!strcmp(program,"focus")) {
		focus_program(virtmem,npages*PAGE_SIZE);

	} else {
		fprintf(stderr,"unknown program: %s\n",program);

	}


	printf("page faults: %d\n",page_fault);
	printf("number of disk writes: %d\n", num_disk_write);
	printf("number of disk reads: %d\n", num_disk_read);
	page_table_delete(pt);
	disk_close(disk);

	return 0;
}
