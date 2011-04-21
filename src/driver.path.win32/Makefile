# Makefile for Windows PATH instrumentation driver

.PHONY: all clean install
all: capture.exe
clean:
	rm -f capture.exe *.obj
install: all
	cp -f begin release capture.exe capture.sh "$(DEST)"

capture.exe: capture.c
	cl.exe -o $@ $^
