EXTRA_CFLAGS = -Wall -g
obj-m = module1.o
 
KDIR = /lib/modules/`uname -r`/build

kbuild:
	make -C $(KDIR) M=`pwd`

clean:
	make -C $(KDIR) M=`pwd` clean