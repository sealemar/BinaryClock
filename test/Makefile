# developed by Sergey Markelov (11/10/2013)
#

.PHONY: lib

PROG       := tests
CTAGS_FILE := ../etc/test.tags
CTAGS_DIR  := ../test

all: $(PROG)

include ../include/Makefile.include

$(PROG): lib

lib:
	make -C ../lib
