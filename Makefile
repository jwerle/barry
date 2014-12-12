
## program name
BIN ?= barry

## program prefix install path
PREFIX ?= /usr/local

## program main
MAIN = main.c

## program source
SRC = $(filter-out $(MAIN), $(wildcard *.c))

## program source dependencies
SRC += $(wildcard deps/fs/*.c)

## program objects
OBJS = $(SRC:.c=.o)

CFLAGS += -Ideps

.PHONY: $(BIN)
$(BIN): $(OBJS)
	$(CC) $(OBJS) $(MAIN) $(CFLAGS) -o $(@)

.PHONY: $(OBJS)
$(OBJS):
	$(CC) -c $(@:.o=.c) -o $(@)

clean:
	rm -f $(OBJS) $(BIN)

install: $(BIN)
	install $(BIN) $(PREFIX)/bin

uninstall:
	rm -f $(PREFIX)/bin/$(BIN)

