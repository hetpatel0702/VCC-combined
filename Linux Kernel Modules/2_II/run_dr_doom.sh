#!/bin/bash

## Building the applications
make 

## Build your ioctl driver and load it here
sudo insmod module2.ko
sudo chmod 777 /dev/query

###############################################

# Launching the control station
./control_station &
c_pid=$!

echo "---------------------"
echo ""

echo "Control station PID: $c_pid"
sleep 3
# Launching the soldier
./soldier $c_pid &
echo "Soldier PID: $!"

sleep 2
kill -9 $c_pid

## Remove the driver here
sudo rmmod module2.ko

