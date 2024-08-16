#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{

    printf("PID: %d\n", getpid());

    // Allocates multiple of 1000 pages each time
    for(int i=1;i<5;i++)
    {
        int *x = (int *)calloc(i * 1000, 4096);
        printf("Allocated...\n");
        sleep(30);
        free(x); 
    }
}