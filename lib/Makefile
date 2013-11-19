#
# developed by Sergey Markelov (11/10/2013)
#

PROG           := libclock.a
CTAGS_FILE     := ../etc/lib.tags
CTAGS_DIR      := ../lib
USE_GENERIC_CC := 0

include ../include/Makefile.include

$(BIN_DIR)/$(PROG): $(OBJECTS)
	$(AR) rvs $@ $?
