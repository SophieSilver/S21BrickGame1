CC = gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Werror
DEBUG_CFLAGS := -g
RELEASE_CFLAGS := -O3
BIN_FLAGS := -lncurses
TEST_FLAGS := -lcheck -lm -lsubunit
SANITIZE_FLAGS := -fsanitize=address,undefined
GCOV_FLAGS := --coverage

DIST_NAME := brick_game-1.0

BUILD_DIR := build
# In real life projects this would probably be something like 
# /usr/local/bin (on linux), but 
# a) it requires sudo 
# b) I don't wanna pollute mine (or reviewer's) filesystems
INSTALL_DIR = install
DOC_DIR := doc
GCOV_DIR := report

# Same idea as INSTALL_DIR,
# in real projects this would be something like ~/.local/share/tetris
APPDATA_DIR ?= $(abspath application_data/tetris)

CFLAGS += -D TETRIS_DATA_DIR='"$(APPDATA_DIR)"'

PROFILE ?= debug

ifeq ($(PROFILE), debug)
CFLAGS += $(DEBUG_CFLAGS)
endif
ifeq ($(PROFILE), release)
CFLAGS += $(RELEASE_CFLAGS)
endif
ifeq ($(PROFILE), sanitize)
CFLAGS += $(SANITIZE_FLAGS)
endif
ifeq ($(PROFILE), gcov)
CFLAGS += $(GCOV_FLAGS)
endif

OUT_DIR = $(BUILD_DIR)/$(PROFILE)
OBJ_DIR = $(OUT_DIR)/obj

HEADERS := $(wildcard src/brick_game/tetris/*.h src/gui/cli/*.h \
	src/brick_game/tetris/game/*.h src/brick_game/tetris/test/*.h \
	src/brick_game/*.h)

LIB_SRCS := $(wildcard src/brick_game/tetris/*.c src/brick_game/tetris/game/*.c)
LIB_OBJS = $(LIB_SRCS:%.c=$(OBJ_DIR)/%.o) 

BIN_SRCS := $(wildcard src/gui/cli/*.c)
BIN_OBJS = $(BIN_SRCS:%.c=$(OBJ_DIR)/%.o) 

TEST_SRCS := $(wildcard src/brick_game/tetris/test/*.c)
TEST_OBJS = $(TEST_SRCS:%.c=$(OBJ_DIR)/%.o)

ALL_SRCS := $(LIB_SRCS) $(BIN_SRCS) $(TEST_SRCS)
ALL_OBJS := $(LIB_OBJS) $(BIN_OBJS) $(TEST_OBJS)
ALL_DEPS := $(ALL_OBJS:%.o=%.d)

LIB := libtetris.a
BIN := tetris

.PHONY: all run run_valgrind clean test test_valgrind \
	test_sanitize test_leaks gcov_report clang_format cppcheck \
	install uninstall dist doc

all: $(OUT_DIR)/$(BIN)

run: $(OUT_DIR)/$(BIN)
	./$<

run_valgrind: $(BIN)
	@ulimit -n 4096; valgrind --tool=memcheck --leak-check=yes ./$<

run_sanitize: export PROFILE = sanitize
run_sanitize:
	@$(MAKE) run

run_release: export PROFILE = release
run_release:
	@$(MAKE) run

install: export PROFILE = release
install:
	$(MAKE) all
	@mkdir -p $(INSTALL_DIR)
	cp $< $(INSTALL_DIR)

uninstall: 
	rm -f $(INSTALL_DIR)/$(BIN)
	rm -rf $(APPDATA_DIR)

dist:
	/usr/bin/sh ./make_dist.sh $(DIST_NAME)

doc:
	doxygen
	@echo Documentaion can be opened in the browser: ./doc/html/index.html

$(OUT_DIR)/$(BIN): $(BIN_OBJS) $(OUT_DIR)/$(LIB) 
	$(CC) $(CFLAGS) $^ $(BIN_FLAGS) -o $@

test: $(OUT_DIR)/test
	@./$<

gcov_report: export PROFILE = gcov
gcov_report:
	@$(MAKE) test
	lcov -t "report" -c -d $(OUT_DIR) --ignore-errors inconsistent --output-file $(OUT_DIR)/coverage.info
	lcov -e $(OUT_DIR)/coverage.info $(realpath .) --output-file $(OUT_DIR)/coverage.info
	genhtml $(OUT_DIR)/coverage.info --output-directory $(GCOV_DIR)

test_valgrind: $(OUT_DIR)/test
# for some reason on my system vscode sets a really high file descriptor limit
# which breaks valgrind, ulimit fixes it
	@ulimit -n 4096 && valgrind --tool=memcheck --leak-check=yes ./$<

test_leaks: $(OUT_DIR)/test
	@leaks -atExit -- ./$^

test_sanitize: export PROFILE = sanitize
test_sanitize:
	@$(MAKE) test

clang_format_check:
	clang-format -n --style=file:../.clang-format $(HEADERS) $(ALL_SRCS)

clang_format:
	clang-format -i --style=file:../.clang-format $(HEADERS) $(ALL_SRCS)

cppcheck:
	cppcheck --enable=all --std=c11 --check-level=exhaustive \
		--disable=information --suppress=missingIncludeSystem --suppress=missingInclude \
		--suppress=checkersReport --suppress=unusedStructMember $(ALL_SRCS) $(HEADERS)

clean:
	rm -rf $(BUILD_DIR) *.a test report application_data $(INSTALL_DIR) dist $(DOC_DIR)

$(OUT_DIR)/test: $(TEST_OBJS) $(OUT_DIR)/$(LIB)
	$(CC) $(CFLAGS) $^ $(TEST_FLAGS) -o $@

$(OUT_DIR)/$(LIB): $(LIB_OBJS)
	ar rcs $@ $^

# object files depend on the Makefile 
# so all of them rebuild when the Makefile changes
$(OBJ_DIR)/%.o: %.c Makefile
	@mkdir -p $(APPDATA_DIR)
	@mkdir -p $(@D)
# -MMD created .d files for discovering dependencies on header files
	$(CC) $(CFLAGS) -c '$<' -MMD -o '$@'

# include dependencies on header files from .d files
-include $(ALL_DEPS)