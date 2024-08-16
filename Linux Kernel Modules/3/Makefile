EXTRA_CFLAGS = -Wall -g
obj-m = hello_procfs.o get_pgfaults.o

KDIR = /lib/modules/`uname -r`/build

kbuild:
	make -C $(KDIR) M=`pwd`

clean:
	make -C $(KDIR) M=`pwd` clean