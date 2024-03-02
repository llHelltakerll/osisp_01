#makefile

CC = gcc
CFLAGS = -std=c11 -D_GNU_SOURCE -g2 -ggdb -pedantic -W -Wall -Wextra
.SUFFIXES:
.SUFFIXES: .c .o

DEBUG = ./build/debug
RELEASE = ./build/release
OUT_DIR = $(DEBUG)

vpath %.c src
vpath %.h src
vpath %.o debug

ifeq ($(MODE), release)
CFLAGS = -std=c11 -D_GNU_SOURCE -pedantic -W -Wall -Wextra -Werror
OUT_DIR = $(RELEASE)
vpath %.o release
endif

objects = $(OUT_DIR)/main.o $(OUT_DIR)/lib.o
prog = $(OUT_DIR)/test

all: $(prog)

$(prog): $(objects)
	mkdir -p $(DEBUG)
	$(CC) $(CFLAGS) $(objects) -o $@

$(OUT_DIR)/%.o: %.c
	mkdir -p $(DEBUG)
	$(CC) -c $(CFLAGS) $^ -o $@

.PHONY: clean

clean:
	@rm -rf $(DEBUG)/* $(RELEASE)/* test

