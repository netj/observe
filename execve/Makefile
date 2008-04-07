LIBC=$(shell ls /lib/libc.so* | sort -r | head -1)
CFLAGS+=-fPIC -DLIBC='"$(LIBC)"'
CFLAGS+=-g

libobserve.so: execve.o
	$(LD) -o $@ -shared $^ -ldl

