//freeing an already free pointer

#include "../malloc.c"

int main(int argc, char **argv){ 
	int *ptr = (int *)malloc(sizeof(int)); 
	printf("ptr's memory address: %x\n", ptr);
	free(ptr); 
	free(ptr); 
} 
