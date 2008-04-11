# Makefile for Windows PATH instrumentation driver

.PHONY: all clean
all: capture.exe
clean:
	rm -f capture.exe *.obj

capture.exe: capture.c
	cl.exe -o $@ $^
