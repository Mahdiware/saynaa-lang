# Makefile for building shared library

# Compiler and flags
CC = gcc
CFLAGS = -fPIC -I../../../src/cli/ -I../../../src/runtime/
LDFLAGS = -shared

# Source files
SRC = mylib.c nativeapi.c
OBJ = $(SRC:.c=.o)

# Output library
TARGET = mylib.so

# Default target
all: $(TARGET)

# Rule to build the shared library
$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean