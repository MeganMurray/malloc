/*#include <stdio.h>
#include "malloc.h"

int main() {

    char* ptr = (char*) malloc(13 * sizeof(char));

    ptr[0] = 'h';
    ptr[1] = 'e';
    ptr[2] = 'l';
    ptr[3] = 'l';
    ptr[4] = 'o';
    ptr[5] = ' ';
    ptr[6] = 'w';
    ptr[7] = 'o';
    ptr[8] = 'r';
    ptr[9] = 'l';
    ptr[10] = 'd';
    ptr[11] = '!';
    ptr[12] = '\0';

    printf("%s\n", ptr);
    
    printf("\n---- Running test1 ---\n");
    
    int * p = (int *) malloc( 20 );
    *p = 10;
    free( p );
    
    p = (int *) malloc( 20 );
    *p = 10;
    free( p );
    
    printf(">>>> test1 passed\n\n");
    
    printf("\n---- Running test2 ---\n");
    
    int allocations = 1000;
    int i = 1;
    for (;i < allocations; i++ ) {
        char * p1 = (char *) malloc( i );
        *p1 = 100;
        char * p2 = (char *) malloc( 100 );
        free( p1 );
        free( p2 );
    }
    char * p3 = (char*) malloc( 100000 );
    *p3 = 0;
    printf(">>>> test2 passed\n\n");
    
    printf("\n---- Running test5 ---\n");
    
    int * a = (int *) malloc( 100 * 1024 - 8);
    //int * b = (int *) malloc( 10 * 1024 - 8 );
    int * c = (int *) malloc( 50 * 1024 - 8 );
    
    free( a );
    free( c );
    
    c = (int *) malloc( 50 * 1024 - 8 );
    a = (int *) malloc( 100 * 1024 - 8 );
    
    printf(">>>> test5 passed\n\n");
 

    return 0;
}
*/

#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>


// Define the color codes as macros
#define RESET_COLOR "\e[m"
#define MAKE_GREEN "\e[32m"
#define MAKE_BLUE "\e[36m"


int main(void)
{
    char *curr_dir = NULL;
    DIR *dp = NULL;
    struct dirent *dptr = NULL;
    unsigned int count = 0;
    
    // Find the column width of terminal
    // We will make use of this in part-II
    // Of this article.
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    
    curr_dir = getenv("PWD");
    if(NULL == curr_dir)
    {
        printf("\n ERROR : Could not get the working directory\n");
        return -1;
    }
    
    dp = opendir((const char*)curr_dir);
    if(NULL == dp)
    {
        printf("\n ERROR : Could not open the working directory\n");
        return -1;
    }
    
    for(count = 0; NULL != (dptr = readdir(dp)); count++)
    {
        if(dptr->d_name[0] != '.')
        {
            // Check if the file is executable
            if(!access(dptr->d_name,X_OK))
            {
                int fd = -1;
                struct stat st;
                
                fd = open(dptr->d_name, O_RDONLY, 0);
                if(-1 == fd)
                {
                    printf("\n Opening file/Directory failed\n");
                    return -1;
                }
                
                fstat(fd, &st);
                // Check if it actaully was a directory with execute
                // permissions on it.
                if(S_ISDIR(st.st_mode))
                {
                    // If it was a directory, print it in Blue
                    printf(MAKE_BLUE"%s     "RESET_COLOR,dptr->d_name);
                }
                else
                {
                    // If it was a normal executable
                    // Print it in green
                    printf(MAKE_GREEN"%s     "RESET_COLOR,dptr->d_name);
                }
                close(fd);
            }
            else
            {
                // No executable flag ON
                // Print it in black(default)
                printf("%s     ",dptr->d_name);
            }
        }
    }
    printf("\n");
    
    return 0;
}