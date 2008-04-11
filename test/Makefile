# Makefile for testing observe
# Author: Jaeho Shin <jae.shin@samsung.com>
# Created: 2008-04-11

TC=\
   execve	\
   execv	\
   execvp	\
   execle	\
   execl	\
   execlp	\
   system	\
   cute		\
   #
PASSES=$(TC:%=%/passed)


.PHONY: test clean
test: $(PASSES)
clean:
	rm -f $(PASSES)

%/passed: %/expected %/actual
	cd $(@D) && diff -u expected actual && touch passed
	@echo "$(@D) PASSED"

%/actual: %/run
	cd $(@D) && ../driver ./run

%/run: %/run.c
	$(CC) -o $@ $^

