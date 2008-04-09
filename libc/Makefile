LIBC_PATH=$(shell ls /lib/libc.so* | sort -r | head -1)

#CFLAGS+=-DDEBUG -g
CFLAGS+=-I. -fPIC -rdynamic -Wall -DLIBC_PATH='"$(LIBC_PATH)"'
LDFLAGS+=-shared -rdynamic

.PHONY: all clean
all: libobserve.so enter
clean:
	rm -f libobserve.so enter *.o

libobserve.so: execve.o
	$(CC) -o $@ $(LDFLAGS) $^ -lc -ldl
execve.c: observe.h

enter: enter.o
	$(CC) -o $@ $^
enter.c: observe.h

observe.h: Makefile
