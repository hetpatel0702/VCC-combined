#ifndef QUERY_IOCTL_H
#define QUERY_IOCTL_H
 
typedef struct
{
    int PID;
} cs_pid;

// Define IOCTL command
#define SEND_CONTROL_STATION_PID _IOWR('q', 1, cs_pid *)
#endif