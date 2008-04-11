# Makefile for observe
# Author: Jaeho Shin <netj@sparcs.org>
# Created: 2008-04-07
VERSION=1.3
DRIVERS=

EXECS=\
	main			\
	compile			\
	handle			\
	usage			\
	version			\
	default			\
	#
FILES=\
	#

.PHONY: all test clean driver
all: driver observe
driver: default
test:
	$(MAKE) -C test
clean:
	$(MAKE) -C test clean
	rm -f observe version default


# choose platform specific drivers
UNAME=$(shell uname)
ifneq ($(filter CYGWIN%,$(UNAME)),)
    DEFAULT_DRIVER=path.win32
    DRIVERS+=path.win32
else
    DRIVERS+=path
    DEFAULT_DRIVER=path
    ifneq ($(filter Linux SunOS,$(UNAME)),)
        DRIVERS+=libc
        DEFAULT_DRIVER=libc
    endif
endif

# POSIX PATH instrumentation driver
ifneq ($(filter path,$(DRIVERS)),)
EXECS+=\
	path/begin		\
	path/release		\
	path/capture		\
	#
endif

# Windows PATH instrumentation driver
ifneq ($(filter path.win32,$(DRIVERS)),)
EXECS+=\
	path.win32/begin	\
	path.win32/release	\
	path.win32/capture.exe	\
	path.win32/capture.sh	\
	#
driver: path.win32-driver
path.win32-driver:
	$(MAKE) -C path.win32
clean: path.win32-clean
path.win32-clean:
	$(MAKE) -C path.win32 clean
endif

# C library's execve(2) and exec*(3) replacement driver
ifneq ($(filter libc,$(DRIVERS)),)
EXECS+=\
	libc/begin		\
	libc/release		\
	libc/capture.so		\
	#
driver: libc-driver
libc-driver:
	$(MAKE) -C libc
clean: libc-clean
libc-clean:
	$(MAKE) -C libc clean
endif


# main build rule
observe: $(EXECS) $(FILES)
	chmod +x $(EXECS)
	pojang $(EXECS) $(FILES) >$@
	chmod +x $@

default:
ifdef DEFAULT_DRIVER
	ln -sfn $(DEFAULT_DRIVER) default
endif

version: Makefile
	echo "#!/bin/sh" 					 >$@
	echo "echo \"observe -- a process invocation observer\"">>$@
	echo "echo"						>>$@
	echo "echo \"Version: $(VERSION)\""			>>$@
	echo "echo"						>>$@
	echo "echo Supported drivers:"				>>$@
	for drv in $(DRIVERS); do echo "echo +$$drv"; done	>>$@
	echo "echo"						>>$@
	echo "echo \"Built on:\""				>>$@
	echo "echo \"`uname -srmp`\""				>>$@
	chmod +x $@

