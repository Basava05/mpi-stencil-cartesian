MPICC ?= mpicc
CFLAGS ?= -O3 -std=c11 -Wall -Wextra -Iinclude
LDFLAGS ?= -lm
SRC := $(wildcard src/*.c)
BIN := build/mpi_stencil
.PHONY: all clean
all: $(BIN)
$(BIN): $(SRC) | build
	$(MPICC) $(CFLAGS) $(SRC) -o $@ $(LDFLAGS)
build:
	mkdir -p build
clean:
	rm -rf build
