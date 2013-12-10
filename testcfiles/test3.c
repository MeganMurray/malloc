//malloc and frees pointers, printing out memory addresses.  More complicated version of test2

#include "../malloc.c"

int main(int argc, char **argv){ 
	int *a = (int *)malloc(sizeof(int));
	char *b = (char *)malloc(sizeof(char)); 
	
	printf("a's memory address: %x\n", a); 
	printf("b's memory address (before freeing a): %x\n", b);
	free(a);
	printf("b's memory address (after freeing a): %x\n", b); 

	char *c  = (char *)malloc(sizeof(char)); 
	printf("c's memory address (allocated after freeing a, but before freeing b): %x\n", c); 

	int *d = (int *)malloc(sizeof(char)); 
	printf("d's memory address (allocated before freeing b and c): %x\n", d);
	free(c); 
	free(d); 

	char *e = (char *)malloc(sizeof(char)); 
	printf("e's memory address (allocated after freeing b, c and d: %x\n", e);
	
	return 0; 
}
