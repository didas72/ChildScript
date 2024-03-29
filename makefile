CC=gcc
C_FLAGS=-O2 -Wall -Wextra -Wno-unknown-pragmas
DBG_FLAGS=-Wall -Wextra -Wno-unknown-pragmas -ggdb
GDB_FLAGS=-ex "b main"

VAL_FLAGS=--leak-check=full --show-leak-kinds=all --track-origins=yes -s

DIR_SRC=src
DIR_INC=include
DIR_LIB=lib
DIR_BUILD=build

LIBS=

OUT_BIN=$(DIR_BUILD)/bin/main
DBG_BIN=$(DIR_BUILD)/bin/dbg

SRCS=$(shell find $(DIR_SRC)/ -type f -name '*.c')
LIB_FLAGS=$(patsubst %,-l%,$(LIBS))

RUN_ARGS=examples/hello.chs


.PHONY: all build run dbg val clean


all: build
build: $(OUT_BIN)



run: $(OUT_BIN)
	./$(OUT_BIN) $(RUN_ARGS)

$(OUT_BIN): $(SRCS)
	@mkdir -p $(@D)
	$(CC) $(C_FLAGS) -I$(DIR_INC) -L$(DIR_LIB) -o $@ $^ $(LIB_FLAGS)

$(DBG_BIN): $(SRCS)
	@mkdir -p $(@D)
	$(CC) $(DBG_FLAGS) -I$(DIR_INC) -L$(DIR_LIB) -o $@ $^ $(LIB_FLAGS)

dbg: $(DBG_BIN)
	gdb $(GDB_FLAGS) ./$(DBG_BIN)

val: $(DBG_BIN)
	valgrind $(VAL_FLAGS) ./$(DBG_BIN)

clean:
	$(RM) -r $(DIR_BUILD)

