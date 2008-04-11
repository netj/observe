# Makefile for C library instrumentation driver

.PHONY: all clean
all: capture.so release
clean:
	rm -f capture.so release *.o


CFLAGS+=-I. -fPIC -DLIBC_PATH='"$(LIBC_PATH)"'
ifdef DEBUG
CFLAGS+=-g -DDEBUG
endif


observe.h: Makefile

release.o: observe.h
release: release.o
	$(CC) -o $@ $^

# OS specific stuffs
UNAME=$(shell uname)

# Linux
ifneq ($(filter Linux,$(UNAME)),)
CFLAGS+=-Wall
LIBC_PATH=$(shell ls /lib/libc.so* | sort -r | head -1)
linux.o: observe.h
capture.so: linux.o
	$(CC) -o $@ -shared -rdynamic $(LDFLAGS) $^ -ldl
endif

