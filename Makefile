
## Copyright (c) 2022-2023 Mohamed Abdifatah. All rights reserved.
## Distributed Under The MIT License

## PROGRAM NAME
NAME = saynaa

## MODE can be DEBUG or RELEASE
MODE = DEBUG

CC        = gcc
CCFLAGS   = -fPIC -MMD -MP
LDFLAGS   = -lm -ldl
OBJ_DIR   = obj/

SRC  = src/cli/        \
       src/compiler/   \
       src/optionals/  \
       src/runtime/    \
       src/shared/     \
       src/utils/      \

SRCS  := $(foreach DIR,$(SRC),$(wildcard $(DIR)*.c))
OBJS  := $(addprefix $(OBJ_DIR), $(SRCS:.c=.o))

ifeq ($(MODE),DEBUG)
	CFLAGS = $(CCFLAGS) -DDEBUG -g3 -Og
else ifeq ($(MODE),RELEASE)
	CFLAGS = $(CCFLAGS) -g -O3
else
	CFLAGS = $(CCFLAGS)
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
