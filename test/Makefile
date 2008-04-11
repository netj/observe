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
PASSES=$(TC:%=%/ok)


.PHONY: test clean
test: ../observe $(PASSES)
clean:
	rm -f $(PASSES)
	for mf in */Makefile; do d=$${mf%/Makefile}; $(MAKE) -C $$d clean; done

../observe:
	$(MAKE) -C ..

%/ok: %/expected %/actual
	cd $(@D) && diff -u expected actual && touch ok
	# $(@D) PASSED

%/actual: %/run
	[ -x driver ] || chmod +x driver
	cd $(@D) && ../driver ./run

%/run: %/run.c
	$(CC) -o $@ $^


%/run %/expected: %/Makefile
	$(MAKE) -C $(@D) $(@F)

