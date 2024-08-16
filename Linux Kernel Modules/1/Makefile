EXTRA_CFLAGS = -Wall -g
obj-m = lkm1.o lkm2.o lkm3.o lkm4.o
 
KDIR = /lib/modules/`uname -r`/build

kbuild:
	make -C $(KDIR) M=`pwd`

clean:
	make -C $(KDIR) M=`pwd` clean