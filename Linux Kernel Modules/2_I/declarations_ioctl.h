#ifndef QUERY_IOCTL_H
#define QUERY_IOCTL_H
 
typedef struct
{
    int PID;
    unsigned long int virtual_address;
    unsigned long int physical_address;
} virt_addr;

typedef struct
{
    unsigned long int physical_address;
    char value;
} phys_value;

// Define IOCTL commands
#define GET_PHYSICAL_ADDRESS _IOWR('q', 1, virt_addr *)
#define PASS_PHYSICAL_ADDRESS_AND_VALUE _IOWR('q', 2, phys_value *) 
#endif