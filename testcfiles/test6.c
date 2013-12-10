//Freeing data that was never allocated by malloc

#include "../malloc.c"

int main(int argc, char **argv){ 
	int x = 3; 
	printf("Freeing data that was never allocated by malloc\n");
	free(x); 

} 
