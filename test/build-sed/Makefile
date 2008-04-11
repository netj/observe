NAME=sed-4.1.5
TARBALL=$(NAME).tar.gz
GNUFTP=http://ftp.kaist.ac.kr/pub/gnu

run: build build/config.status
	cp -f run.in $@
	chmod +x $@

.PHONY: clean distclean
clean:
	rm -rf $(NAME) build run
distclean:
	rm -f $(TARBALL)

build/config.status:
	cd build && ./configure

build: $(NAME)
	ln -s $< build

$(NAME): $(TARBALL)
	gunzip <$< | tar xf -

$(TARBALL):
	wget $(GNUFTP)/sed/$@

