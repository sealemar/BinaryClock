#
# developed by Sergey Markelov (11/10/2013)
#

ifndef ARDMK_DIR
    $(error "ARDMK_DIR is not defined")
endif

#
# Arduino as ISP programmer
#
ISP_PROG	  := stk500v1
ISP_PORT      = /dev/cu.usb*
AVRDUDE_ISP_BAUDRATE := 19200
AVRDUDE_OPTS  := -v

#
# General info
#
BOARD_TAG     = uno
MONITOR_PORT  = /dev/cu.usb*
ARDUINO_LIBS  = lib
USER_LIB_PATH := $(realpath ..)
MONITOR_CMD   = screen -c /dev/null
CPPFLAGS      = -include ../include/common.h -include stdint.h
CFLAGS        = -std=c99 -include stddef.h

include $(ARDMK_DIR)/Arduino.mk

#
# Override Arduino.mk defaults to point to my soft links to
# colorgcc. Colorgcc reads the config from ~/.colorgccrc
#
CC = avr-gcc
CXX = avr-g++


.PHONY: ctags remove-ctags
CTAGS_SOURCES = $(wildcard *.ino) $(wildcard *.[ch])
CTAGS         = ctags --sort=yes --fields=+ilaS --extra=+q
CTAGS_FILE   := ../etc/arduino.tags

clean: remove-ctags

remove-ctags:
	rm -f $(CTAGS_FILE)

ctags: $(CTAGS_FILE)

$(CTAGS_FILE):
	@mkdir -p ../etc
	$(CTAGS) --language-force=C --c-kinds=+px -f $@ $(CTAGS_SOURCES)
