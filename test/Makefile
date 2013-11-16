# developed by Sergey Markelov (11/10/2013)
#

.PHONY: lib

INC          := -include errno.h
POST_INCLUDE := -include logger.h -include test_ut/test_ut.h
SOURCES_DIRS  = arch test_ut
PROG         := tests
CTAGS_FILE   := ../etc/test.tags
CTAGS_DIR    := ../test

include ../include/Makefile.include

$(BIN_DIR)/$(PROG): lib

lib:
	make -C ../lib
