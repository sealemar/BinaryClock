#
# developed by Sergey Markelov (11/10/2013)
#

PROG           := libclock.a
SOURCES_DIRS    = arch
CTAGS_FILE     := ../etc/lib.tags
CTAGS_DIR      := ../lib

all: $(BIN_DIR)/$(LIBRARY)

include ../include/Makefile.include

$(BIN_DIR)/$(PROG): $(OBJECTS)
	mkdir -p $(BIN_DIR)
	$(AR) -rvs $@ $?
