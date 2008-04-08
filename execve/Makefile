LIBC=$(shell ls /lib/libc.so* | sort -r | head -1)
CFLAGS+=-fPIC -DLIBC='"$(LIBC)"'
CFLAGS+=-g

libobserve.so: execve.o
	$(CC) -o $@ -shared $^ -lc -ldl

.PHONY: clean
clean:
	rm -f libobserve.so *.o
