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

void page_fault_handler( struct page_table *pt, int page )
{
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
				printf("optarg de n: %s\n", optarg);
				npages = atoi(optarg);
			case 'f':
				nframes = atoi(optarg);
			case 'a':
				swaptype = optarg;
			case 'p':
				program = optarg;

		}
	}
		
	printf ("swaptype: %s\n", swaptype);
	printf ("progeam: %s\n", program);
	printf ("npages: %i\n", npages);
	printf ("nframes: %i\n", nframes);
	//int **marcos = 

	struct disk *disk = disk_open("myvirtualdisk",npages);
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
		fprintf(stderr,"unknown program: %s\n",argv[3]);

	}

	page_table_delete(pt);
	disk_close(disk);

	return 0;
}
