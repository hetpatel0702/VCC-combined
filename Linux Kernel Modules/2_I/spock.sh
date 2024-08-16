#! /bin/bash

make
sudo insmod module1.ko
sudo chmod a+rwx /dev/query
gcc user.c -o user
echo "-------------------------"
echo ""
sudo ./user
sudo rmmod module1.ko
