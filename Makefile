
## Copyright (c) 2022-2026 Mohamed Abdifatah. All rights reserved.
## Distributed Under The MIT License

## PROGRAM NAME
NAME = saynaa

## MODE can be DEBUG or RELEASE
## READLINE can be enable or disable
MODE 	 = DEBUG
READLINE = enable

CC        = gcc
CCFLAGS   = -fPIC -MMD -MP
LDFLAGS   = -lm -ldl -lpcre2-8
OBJ_DIR   = obj/

# Recursively find all C files in src
SRCS := $(shell find src -name "*.c")
OBJS  := $(addprefix $(OBJ_DIR), $(SRCS:.c=.o))
SOLIB = libsaynaa.so

# Exclude CLI from shared library
LIB_SRCS = $(filter-out src/cli/saynaa.c, $(SRCS))
LIB_OBJS = $(addprefix $(OBJ_DIR), $(LIB_SRCS:.c=.o))

ifneq ($(MODE),RELEASE)
	CFLAGS += $(CCFLAGS) -DDEBUG -g3 -Og
else
	CFLAGS += $(CCFLAGS) -g -O3
endif

# TODO: MacOS don't impelement shared library properly yet
UNAME_S := $(shell uname -s)
BUILD_SHARED = no

ifeq ($(UNAME_S),Darwin)
    # Add typical Homebrew include paths
    CFLAGS += -I/opt/homebrew/include -I/usr/local/include
    LDFLAGS += -L/opt/homebrew/lib -L/usr/local/lib
else ifeq ($(UNAME_S),Linux)
    LDFLAGS += -Wl,--export-dynamic
    BUILD_SHARED = yes
endif

ifeq ($(READLINE),enable)
    CFLAGS += -DREADLINE
	LDFLAGS += -lreadline
endif

.PHONY: all clean

$(NAME): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $^ -o $@ $(LDFLAGS)

ifeq ($(BUILD_SHARED),yes)
$(SOLIB): $(LIB_OBJS)
	@mkdir -p $(dir $@)
	$(CC) -shared -o $@ $^ $(LDFLAGS)
endif

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
