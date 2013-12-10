#include "malloc.c"

int main(int argc, char **argv){ 
	int *ptr = (int *)malloc(sizeof(int)); 
	printf("Memory Address: %x\n", ptr);
	free(ptr);
}
