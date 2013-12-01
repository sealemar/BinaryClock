#
# developed by Sergey Markelov (11/10/2013)
#

# for localtime_r() on Linux
MACROS      := -D _BSD_SOURCE

PROG        := terminal-binary-clock
LIBS         = -lncurses
CTAGS_FILE  := ../etc/emulator.tags
CTAGS_DIR   := ../emulator

include ../include/Makefile.include

$(BIN_DIR)/$(PROG): lib

lib:
	make -C ../lib
