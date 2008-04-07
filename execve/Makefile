LIBC=$(shell ls /lib/libc.so* | sort -r | head -1)
CFLAGS+=-DLIBC='"$(LIBC)"' -g

libobserve.so: execve.o
	$(LD) -o $@ -shared $^ -ldl

