#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/ioctl.h>
// Include header or define the IOCTL call interface and devide name
#include "declarations_ioctl_2.h"

//**************************************************

int open_driver(const char* driver_name) {

    int fd_driver = open(driver_name, O_RDWR);
    if (fd_driver == -1) {
        perror("ERROR: could not open driver");
    }

	return fd_driver;
}

void close_driver(const char* driver_name, int fd_driver) {

    int result = close(fd_driver);
    if (result == -1) {
        perror("ERROR: could not close driver");
    }
}


int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Usage: %s <parent_pid>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t parent_pid = atoi(argv[1]);
    
    // Open character device file
    int fd = open("/dev/query", O_RDWR);

    // open ioctl driver
    cs_pid q;
    q.PID = parent_pid;
    
    // Send control station's PID to driver
    if (ioctl(fd, SEND_CONTROL_STATION_PID, &q) == -1)
    {
        perror("IOCTL Failed");
    }
    
    // call ioctl with parent pid as argument to change the parent

	
    // close ioctl driver
    close(fd);
	return EXIT_SUCCESS;
}


