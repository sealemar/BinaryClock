CC   := clang -std=c99
INC  := -I../lib
MACROS := -D _BSD_SOURCE
CC_c := $(CC) $(INC) $(MACROS)
CC_LD_FLAGS := -O0 -g
LIBS := -lncurses
PROG := terminal-binary-clock
SOURCES = $(wildcard *.c)
OBJECTS = $(subst .c,.o,$(SOURCES))

.PHONY: all clean

all: $(PROG)

include $(subst .c,.d,$(SOURCES))

%.d: %.c
	$(CC_c) -M $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.o: %.c
	$(CC_c) $(INC) -c -o $@ $<

$(PROG): $(OBJECTS)
	$(CC) $(CC_LD_FLAGS) $(LIBS) -o $@ $^

clean:
	rm -f $(PROG) *.o *.d
