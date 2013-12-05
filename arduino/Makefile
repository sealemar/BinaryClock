#
# developed by Sergey Markelov (11/10/2013)
#

ifndef $(ARDMK_DIR)
    $(error "ARDMK_DIR is not defined")
endif

BOARD_TAG    = uno
MONITOR_PORT = /dev/cu.usb*
ARDUINO_LIBS =
MONITOR_CMD  = screen -c /dev/null

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
