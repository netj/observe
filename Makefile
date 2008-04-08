# Makefile for observe
# Author: Jaeho Shin <netj@sparcs.org>
# Created: 2008-04-07
VERSION=1.0
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

.PHONY: all clean driver
all: driver observe
driver: default
clean:
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
        DRIVERS+=execve
        DEFAULT_DRIVER=execve
    endif
endif

# POSIX PATH instrumentation driver
ifneq ($(filter path,$(DRIVERS)),)
EXECS+=\
	path/prepare		\
	path/run		\
	path/hook		\
	#
endif

# Windows PATH instrumentation driver
ifneq ($(filter path.win32,$(DRIVERS)),)
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
ifneq ($(filter execve,$(DRIVERS)),)
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

