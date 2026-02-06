
## Copyright (c) 2022-2023 Mohamed Abdifatah. All rights reserved.
## Distributed Under The MIT License

## PROGRAM NAME
NAME = saynaa

## MODE can be DEBUG or RELEASE
MODE 	 = DEBUG
READLINE = disable

CC        = gcc
CCFLAGS   = -fPIC -MMD -MP
LDFLAGS   = -lm -ldl -lpcre2-8
OBJ_DIR   = obj/

# Recursively find all C files in src
SRCS := $(shell find src -name "*.c")
OBJS  := $(addprefix $(OBJ_DIR), $(SRCS:.c=.o))

ifneq ($(MODE),RELEASE)
	CFLAGS += $(CCFLAGS) -DDEBUG -g3 -Og
else
	CFLAGS += $(CCFLAGS) -g -O3
endif

# Check for Mac (Darwin) and add include paths for Homebrew
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    # Add typical Homebrew include paths
    CFLAGS += -I/opt/homebrew/include -I/usr/local/include
    LDFLAGS += -L/opt/homebrew/lib -L/usr/local/lib
endif

ifeq ($(READLINE),enable)
    CFLAGS += -DREADLINE
	LDFLAGS += -lreadline
endif

.PHONY: all clean

$(NAME): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

install:
	@cp -r $(NAME) /usr/local/bin/
	@printf "\033[38;5;52m\033[43m\t    installed!    \t\033[0m\n";

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(NAME)
