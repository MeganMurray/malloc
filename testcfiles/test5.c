#include "../malloc.c"

int main(int argc, char **argv){ 
	int *ptr = (int *)malloc(sizeof(int)); 
	printf("ptr's memory address: %x\nGoing to attempt to free %x\n", ptr, ptr+1);
	free(ptr + 1); 
} 
