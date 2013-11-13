#
# developed by Sergey Markelov (11/10/2013)
#

PROG           := libclock.a
SOURCES_DIRS    = arch
CTAGS_FILE     := ../etc/lib.tags
CTAGS_DIR      := ../lib

include ../include/Makefile.include

$(BIN_DIR)/$(PROG): $(OBJECTS)
	$(AR) -rvs $@ $?
