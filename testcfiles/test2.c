//check that it works, mallocs and frees a pointer twice, uses same memory block

#include "../malloc.c"

int main(int argc, char **argv){ 
	int *ptr1 = (int *)malloc(sizeof(int)); 
	printf("ptr1's memory location: %x\n", ptr1); 
	free(ptr1); 

	char *ptr2 = (char *)malloc(sizeof(char)); 
	printf("ptr2's memory location: %x\n", ptr2); 
	free(ptr2); 

} 
