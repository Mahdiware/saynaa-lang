## Copyright (c) 2022-2026 Mohamed Abdifatah. All rights reserved.
## Distributed Under The MIT License

## PROGRAM NAME
NAME = saynaa

## MODE can be DEBUG or RELEASE
## READLINE can be enable or disable
MODE 	      ?= DEBUG
COMPUTED_GOTO ?= enable
READLINE       = enable

CC             = gcc
CCFLAGS        = -fPIC -MMD -MP
LDFLAGS        = -lm -ldl -lpcre2-8
OBJ_DIR        = obj/

# 1. Dynamically check if the optionals folder exists
ifeq ($(wildcard src/optionals/.),)
    # Folder is missing! Exclude optionals from compilation and pass the macro
    CCFLAGS += -DNO_OPTIONALS
    SRCS := $(shell find src -name "*.c" ! -path "src/optionals/*")
else
    # Folder exists! Include everything normally
    SRCS := $(shell find src -name "*.c")
endif

OBJS  := $(addprefix $(OBJ_DIR), $(SRCS:.c=.o))
SOLIB = libsaynaa.so

# Exclude CLI from shared library
LIB_SRCS = $(filter-out src/saynaa/saynaa.c, $(SRCS))
LIB_OBJS = $(addprefix $(OBJ_DIR), $(LIB_SRCS:.c=.o))

# 2. Extract matching dependency (.d) tracking files from our objects
DEPS := $(OBJS:.o=.d)

ifneq ($(MODE),RELEASE)
	CFLAGS += $(CCFLAGS) -DDEBUG -g3 -Og
else
	CFLAGS += $(CCFLAGS) -DNDEBUG -O3 -march=native -flto=auto -fuse-linker-plugin
	LDFLAGS += -flto=auto
endif

# TODO: MacOS don't impelement shared library properly yet
UNAME_S := $(shell uname -s)
UNAME_O := $(shell uname -o)
BUILD_SHARED = no

ifeq ($(UNAME_S),Darwin)
    # Add typical Homebrew include paths
    CFLAGS += -I/opt/homebrew/include -I/usr/local/include
    LDFLAGS += -L/opt/homebrew/lib -L/usr/local/lib
else ifeq ($(UNAME_S),Linux)
    LDFLAGS += -Wl,--export-dynamic
    BUILD_SHARED = yes
    
    ifeq ($(UNAME_O),Android)
        LDFLAGS += -llog
    endif
endif

ifeq ($(READLINE),enable)
    CFLAGS += -DREADLINE
	LDFLAGS += -lreadline
endif

ifneq ($(COMPUTED_GOTO), enable)
    CFLAGS += -DNO_COMPUTED_GOTO
endif

.PHONY: all clean release perf benchmark benchmark-ci benchmark-compare

BENCH_APP1 ?= ./$(NAME)
BENCH_APP2 ?= /usr/local/bin/$(NAME)

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

# 3. Include the generated dependency trackers into the system
# The hyphen '-' quietly skips errors on fresh builds when no files exist yet
-include $(DEPS)

all: $(NAME)

release:
	$(MAKE) MODE=RELEASE all

perf: release

no-computed-goto:
	$(MAKE) COMPUTED_GOTO=disable

benchmark: release
	python3 util/run.py --app ./$(NAME)

benchmark-ci: release
	python3 util/run.py --app ./$(NAME) --warmup 1 --iterations 3 --json-out test/benchmark/results/ci-latest.json

benchmark-compare: release
	python3 util/compare.py --app1 "$(BENCH_APP1)" --app2 "$(BENCH_APP2)"

install:
	@cp -r $(NAME) /usr/local/bin/
	@printf "\033[38;5;52m\033[43m\t    installed!    \t\033[0m\n";

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(NAME)
