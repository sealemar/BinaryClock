#
# developed by Sergey Markelov (11/10/2013)
#

LIBRARY        := libclock.a
FILES_TO_CLEAN  = $(LIBRARY)
SOURCES         = $(wildcard arch/*.c)
CTAGS_FILE     := ../etc/lib.tags
CTAGS_DIR      := ../lib

all: $(LIBRARY)

include ../include/Makefile.include

$(LIBRARY): $(OBJECTS)
	ar -rcs $@ $^
