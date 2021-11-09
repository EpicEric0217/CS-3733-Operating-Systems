#include <stdlib.h>
#include "pagetable.h"
int** initializePT(int*** page_table, int psize) { //function to initialize the page table..
	*page_table = (int **)malloc(psize * sizeof(int*));
	 
	 int x;
	 
	 for(x = 0; x < psize; x++) {
		 (*page_table)[x] = (int *)malloc(2 * sizeof(int));
		 
		 (*page_table)[x][0] = NONE;
		 
		 (*page_table)[x][1] = i;
		 
	 }
	 
	 return *page_table;
	 
}

void freePT(int*** page_table, int psize) {
	int x;
	
	for(x = 0; x < psize; x++)
		
		free((*page_table)[x]);
		
		free(*page_table);
}

int** pointToPage(int*** page_table, int pgvirtual, int framephysc) {
	(*page_table)[pgvirtual][0] = framephysc;
	(*page_table)[pgvirtual][1] = v_i;
	
	
	return *page_table;
	
}
