obj-m += test2.o

all:
	make -C /lib/modules/6.8.0-40-generic/build M=$(PWD) modules
clean:
	make -C /lib/modules/6.8.0-40-generic/build M=$(PWD) clean
