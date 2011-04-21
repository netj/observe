# Makefile for observe
# Author: Jaeho Shin <netj@sparcs.org>
# Created: 2008-04-07

PACKAGENAME:=observe
PACKAGEVERSION:=1.4
PACKAGEEXECUTES:=main
include buildkit/modules.mk

test: all
	$(MAKE) -C test

