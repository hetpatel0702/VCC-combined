#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include "declarations_ioctl.h"
 
unsigned long int get_physical_address(int fd, unsigned long int virtual_address)
{
    virt_addr v;
    v.PID = getpid();
    v.virtual_address = virtual_address;
    v.physical_address = 0;
    
    // Make IOCTL call to get the physical address
    if (ioctl(fd, GET_PHYSICAL_ADDRESS, &v) == -1)
    {
        perror("IOCTL Failed");
    }
    return v.physical_address;
}

void send_physical_address(int fd, unsigned long int physical_address, char value)
{
    phys_value p;

    p.physical_address = physical_address;
    p.value = value;

    // Make IOCTL call to pass physical address
    if (ioctl(fd, PASS_PHYSICAL_ADDRESS_AND_VALUE, &p) == -1)
    {
        perror("IOCTL Failed");
    }
}
 
int main(int argc, char *argv[])
{
    // Open character device file
    int fd = open("/dev/query", O_RDWR);
    if (fd == -1)
    {
        perror("Couldn't Open File");
        return -1;
    }
    
    // Allocate 1B memory
    char *x = (char *)malloc(1);
    *x = 6;

    printf("Virtual Address: %lu\n", x);
    printf("Value: %d\n",*x);

    // Get physical address from kernel
    unsigned long int PA = get_physical_address(fd, (unsigned long int)x);
    printf("Physical address: %lX\n", PA);

    // Send physical address to kernel
    send_physical_address(fd, PA, *x);

    printf("Value: %d\n", *x);
    free(x);
    close (fd);
    return 0;
}
