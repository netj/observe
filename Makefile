# Makefile for observe
# Author: Jaeho Shin <netj@sparcs.org>
# Created: 2008-04-07
VERSION=1.0

EXECS=\
	main			\
	compile			\
	handle			\
	execve/prepare		\
	execve/run		\
	execve/libobserve.so	\
	execve/hook		\
	path/prepare		\
	path/run		\
	path/hook		\
	usage			\
	version			\
	#
FILES=\
	#

observe-$(VERSION).sh: $(EXECS) $(FILES)
	chmod +x $(EXECS)
	pojang $(EXECS) $(FILES) >$@
	chmod +x $@

version:
	{ echo "#!/bin/sh"; echo observe-$(VERSION); } >$@
	chmod +x $@


# libc execve driver
execve/libobserve.so:
	$(MAKE) -C execve


# PATH driver
ifeq ($(shell uname),CYGWIN*)
path/hook: path/hook.c
	$(CC) -o $@ $^
else
path/hook: path/hook.sh
	install -m a+rx $< $@
endif

