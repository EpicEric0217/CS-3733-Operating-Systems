#define v_i 1	
#define i 0
#define NONE -1

int** initializePT(int*** page_table, int psize);
int** pointToPage(int*** page_table, int pgvirtual, int framephysc);
void freePT(int*** page_table, int psize);
