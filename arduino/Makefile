# Copyright [2013] [Sergey Markelov]
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# developed by Sergey Markelov (11/10/2013)
#

ifndef ARDMK_DIR
    $(error "ARDMK_DIR is not defined, probably you forgot to set up https://github.com/sudar/Arduino-Makefile")
endif

#
# Arduino as ISP programmer
#
ISP_PROG	  := stk500v1
ISP_PORT      = /dev/cu.usb*
AVRDUDE_ISP_BAUDRATE := 19200
AVRDUDE_OPTS  := -D -v

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

clean:: remove-ctags

remove-ctags:
	rm -f $(CTAGS_FILE)

ctags: $(CTAGS_FILE)

$(CTAGS_FILE):
	@mkdir -p ../etc
	$(CTAGS) --language-force=C --c-kinds=+px -f $@ $(CTAGS_SOURCES)
