#
# developed by Sergey Markelov (11/10/2013)
#

PROG        := terminal-binary-clock
LIBS         = -lncurses
SOURCES_DIRS = arch
CTAGS_FILE  := ../etc/emulator.tags
CTAGS_DIR   := ../emulator

include ../include/Makefile.include

$(BIN_DIR)/$(PROG): lib

lib:
	make -C ../lib
