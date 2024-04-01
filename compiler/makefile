CC=gcc
C_FLAGS=-O2 -Wall -Wextra -Wno-unknown-pragmas
DBG_FLAGS=-Wall -Wextra -Wno-unknown-pragmas -ggdb
GDB_FLAGS=-ex "set print pretty on"

VAL_FLAGS=--leak-check=full --show-leak-kinds=all --track-origins=yes -s

DIR_SRC=src
DIR_INC=include
DIR_LIB=lib
DIR_BUILD=build
DIR_EMBED=embed

LIBS=

OUT_BIN=$(DIR_BUILD)/bin/chs_compiler
DBG_BIN=$(DIR_BUILD)/bin/chs_compiler_dbg

SRCS=$(shell find $(DIR_SRC)/ -type f -name '*.c')
LIB_FLAGS=$(patsubst %,-l%,$(LIBS))
EMBEDS=$(wildcard $(DIR_EMBED)/*)
EMBED_HEADERS=$(patsubst $(DIR_EMBED)/%, $(DIR_INC)/embed/%.h, $(EMBEDS))

RUN_ARGS=


.PHONY: all build embed_headers run dbg val clean


all: build
build: $(OUT_BIN)
embed_headers: $(EMBED_HEADERS)


run: $(OUT_BIN)
	./$(OUT_BIN) $(RUN_ARGS)

$(OUT_BIN): $(SRCS) $(EMBED_HEADERS)
	@mkdir -p $(@D)
	$(CC) $(C_FLAGS) -I$(DIR_INC) -L$(DIR_LIB) -o $@ $(SRCS) $(LIB_FLAGS)

$(DBG_BIN): $(SRCS) $(EMBED_HEADERS)
	@mkdir -p $(@D)
	$(CC) $(DBG_FLAGS) -I$(DIR_INC) -L$(DIR_LIB) -o $@ $(SRCS) $(LIB_FLAGS)

$(DIR_INC)/embed/%.h: $(DIR_EMBED)/%
	@mkdir -p $(@D)
	xxd -i $< > $@

dbg: $(DBG_BIN)
	gdb $(GDB_FLAGS) ./$(DBG_BIN)

val: $(DBG_BIN)
	valgrind $(VAL_FLAGS) ./$(DBG_BIN)

clean:
	$(RM) -r $(DIR_BUILD)

