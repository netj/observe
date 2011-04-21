# Makefile for C library's execve(2) and exec*(3) replacement driver
# Author: Jaeho Shin <netj@sparcs.org>
# Created: 2008-04-08

.PHONY: all clean install
all: capture.so release
clean:
	rm -f capture.so release *.o
install: all
	cp -f begin release capture.so "$(DEST)"

CFLAGS+=-I. -fPIC -DLIBC_PATH='"$(LIBC_PATH)"'
ifdef DEBUG
CFLAGS+=-g -DDEBUG
endif


release.o: observe.h Makefile
release: release.o
	$(CC) -o $@ $^

capture.o: observe.h Makefile

# OS specific stuffs
UNAME=$(shell uname)

# Linux
ifneq ($(filter Linux,$(UNAME)),)
CFLAGS+=-Wall
LIBC_PATH=$(shell ls /lib/libc.so* | sort -r | head -1)
capture.so: capture.o
	$(CC) -o $@ -shared -rdynamic $(LDFLAGS) $^ -ldl
endif

# Solaris
ifneq ($(filter SunOS,$(UNAME)),)
LIBC_PATH=$(shell ls /lib/libc.so* | sort -r | head -1)
capture.so: capture.o
	$(CC) -o $@ -shared $(LDFLAGS) $^ -ldl
endif
