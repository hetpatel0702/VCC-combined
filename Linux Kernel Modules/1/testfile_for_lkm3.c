#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main()
{ 
    // use this PID and VA for lkm3
    printf("PID: %d\n", getpid());
    int *p = (int *)malloc(5000);
    printf("Virtual Address: %lu\n", p);
    sleep(1000);
}