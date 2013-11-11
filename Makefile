# developed by Sergey Markelov (11/10/2013)
#

.PHONY: arduino emulator check clean ctags

all: arduino emulator

arduino:
	make -C arduino

emulator:
	make -C emulator

check:
	make -C test && ./tests

ctags:
	make -C include -f Makefile.include ctags
	make -C arduino ctags
	make -C emulator ctags
	make -C test ctags
	make -C lib ctags

clean:
	make -C arduino clean
	make -C emulator clean
	make -C test clean
	make -C lib clean
	make -C include -f Makefile.include clean
