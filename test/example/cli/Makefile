# compile saynaa code to libsaynaa.so and run it with the CLI API

# Compiler and flags
CC = gcc
CFLAGS = -I ../../../src
LDFLAGS = -L../../.. -lsaynaa -Wl,-rpath=../../..

# Source files
SRC = main.c
OBJ = $(SRC:.c=.o)

# Output executable
TARGET = cli.app

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJ) $(TARGET)