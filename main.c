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
int *frame_age;
int *frame_use;

int key_frame_oldest ()
{
	int max = 0;
	int key_of_max = 0;
	for_each_item (i, frame_age){
		if(max < i)
			max = i;
	}
	for_each_item (i, frame_age){
		if(i == max){
			break;
		}
		key_of_max++;
	}
	return 
}

bool frame_in_use (int frame){
	for_each_item (i, frame_use){
		if(i == frame)
			return true;
	}
	return false;
}

void replace_frame (int key_of_frame, int page_num){
	frame_use[key_of_frame] = page_num;
	frame_age[key_of_frame] = 0;
}
	

void page_fault_handler( struct page_table *pt, int page )
{

	printf("number of pages in physical memory: %d\n", page_table_get_nframes(pt));
	printf("number of pages in virtual memory: %d\n", page_table_get_npages(pt));
	page_table_print(pt);
	page_table_get_npages(pt);
	disk_write(disk, 1, page_table_get_physmem(pt));
	page_table_set_entry(pt, 0, 0, 0);
	printf("page fault on page #%d\n",page);
	exit(1);
}

int main( int argc, char *argv[] )
{

	int npages;
	int nframes;
	char *swaptype;
	const char *program;
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

	frame_age = malloc(sizeof(int) * )

	disk = disk_open("myvirtualdisk",npages);
	if(!disk) {
		fprintf(stderr,"couldn't create virtual disk: %s\n",strerror(errno));
		return 1;
	}


	struct page_table *pt = page_table_create( npages, nframes, page_fault_handler );
	if(!pt) {
		fprintf(stderr,"couldn't create page table: %s\n",strerror(errno));
		return 1;
	}

	char *virtmem = page_table_get_virtmem(pt);

	char *physmem = page_table_get_physmem(pt);

	if(!strcmp(program,"sort")) {
		sort_program(virtmem,npages*PAGE_SIZE);

	} else if(!strcmp(program,"scan")) {
		scan_program(virtmem,npages*PAGE_SIZE);

	} else if(!strcmp(program,"focus")) {
		focus_program(virtmem,npages*PAGE_SIZE);

	} else {
		fprintf(stderr,"unknown program: %s\n",program);

	}

	page_table_delete(pt);
	disk_close(disk);

	return 0;
}
