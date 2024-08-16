TARGETS = control_station soldier
KDIR = /lib/modules/`uname -r`/build
SOURCES = $(TARGETS:=.c)
EXTRA_CFLAGS = -Wall -g
obj-m = module2.o

all: $(TARGETS) kbuild

$(TARGETS): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $@.c

kbuild:
	make -C $(KDIR) M=`pwd`

clean:
	make -C $(KDIR) M=`pwd` clean
	rm -f $(TARGETS)