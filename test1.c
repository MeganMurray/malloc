#include "malloc.c"

int main(int argc, char **argv){ 
	int *ptr = (int *)malloc(sizeof(int)); 
	*ptr = 3; 
	printf("%d\n", *ptr);
	free(ptr); 
	printf("%d\n", *ptr);
}
