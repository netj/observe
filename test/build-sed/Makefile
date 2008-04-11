GNUFTP=http://ftp.kaist.ac.kr/pub/gnu
NAME=sed-4.1.5
TARBALL=$(NAME).tar.gz


all: run expected

run: rule build build/config.status
	cp run.in $@
	chmod +x $@

expected: expected.$(shell uname)
	ln -s $< $@

rule:
	ln -s ../record-build $@

build: $(NAME)
	ln -s $< build

build/config.status:
	cd build && ./configure

$(NAME): $(TARBALL)
	gunzip <$< | tar xf -

$(TARBALL):
	wget $(GNUFTP)/sed/$@


.PHONY: clean distclean
clean:
	rm -rf $(NAME) build rule expected run
distclean:
	rm -f $(TARBALL)

