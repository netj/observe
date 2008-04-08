# Makefile for observe
# Author: Jaeho Shin <netj@sparcs.org>
# Created: 2008-04-07
VERSION=1.0

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

.PHONY: all clean driver
all: driver observe
driver: default
clean:
	rm -f observe version default


# choose platform specific drivers
UNAME=$(shell uname)
ifneq ($(filter CYGWIN%,$(UNAME)),)
    ENABLE_PATH_WIN32=true
    DEFAULT_DRIVER=path.win32
else
    ENABLE_PATH_POSIX=true
    DEFAULT_DRIVER=path
    ifneq ($(filter Linux SunOS,$(UNAME)),)
        ENABLE_EXECVE=true
        DEFAULT_DRIVER=execve
    endif
endif

# POSIX PATH instrumentation driver
ifdef ENABLE_PATH_POSIX
EXECS+=\
	path/prepare		\
	path/run		\
	path/hook		\
	#
endif

# Windows PATH instrumentation driver
ifdef ENABLE_PATH_WIN32
EXECS+=\
	path.win32/prepare	\
	path.win32/run		\
	path.win32/hook.exe	\
	path.win32/hook.sh	\
	#
driver: path.win32-driver
path.win32-driver:
	$(MAKE) -C path.win32
clean: path.win32-clean
path.win32-clean:
	$(MAKE) -C path.win32 clean
endif

# libc's execve(2) replacement driver
ifdef ENABLE_EXECVE
EXECS+=\
	execve/prepare		\
	execve/run		\
	execve/libobserve.so	\
	execve/hook		\
	#
driver: execve-driver
execve-driver:
	$(MAKE) -C execve
clean: execve-clean
execve-clean:
	$(MAKE) -C execve clean
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
	{ echo "#!/bin/sh"; echo observe-$(VERSION); } >$@
	chmod +x $@

