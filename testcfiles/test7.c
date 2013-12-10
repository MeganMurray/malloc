#include "malloc.c"

int main(int argc, char **argv){ 
	int *ptr; long int x=2000000;
	while(1){
		ptr = (int *)malloc(x * sizeof(int)); 
		if(ptr == NULL){
			printf("FULL!\n"); 
			exit(1); 
		}
		printf("counter: %d == total size: %d == ptr start: %u\n", x, x*sizeof(int), ptr); 
		x++;
	}

} 
