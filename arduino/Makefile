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
