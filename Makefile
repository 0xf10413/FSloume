###
# Variables to set
##################
# Output file name and directory
BIN=sloume
BUILD_DIR=./build

# C/C++ compiler (gcc/clang recommended)
CC=gcc
CXX=g++

# C/C++ compilation flags
CFLAGS=-std=c99 -g -pedantic -Wall -Wextra -Wshadow -Wpointer-arith \
       -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes \
			 -Wfloat-equal -Wundef -Wpointer-arith -Wcast-align \
			 -Wunreachable-code -fPIC -fprofile-arcs -ftest-coverage
CXXFLAGS=-std=c++11 -g -Wall -Wextra -pedantic -Wshadow -Weffc++ \
				 -Wunreachable-code -fPIC -fprofile-arcs -ftest-coverage

# Linker flags
LDFLAGS=-lsfml-system -lsfml-window -lsfml-graphics -lgcov \
				-fprofile-arcs -ftest-coverage


# Resource builder
RC_BUILD = ./tools/objectify.py

# Resource files
RC_ALL=$(wildcard rc/*)
RC_HEADER=game/rc.h
RC_CPP_INTERNAL=game/rc_manager.inc # Beware not to compile it !

# Source files (adjust if needed)
SRC_CXX_GAME=$(wildcard game/*.cpp)
SRC_C_GAME=$(wildcard game/*.c)
SRC_CXX_TEST=$(wildcard tests/*.cpp) $(wildcard tests/*/*.cpp)
SRC_C_TEST=$(wildcard tests/*.c) $(wildcard tests/*/*.c)
SRC_BIN = main.cpp


###
# Automatic variables
#####################

# Test binary
BIN_TEST=$(BIN)_test

# Game lib
LIB_GAME=$(BIN)_game
LIB_GAME_FULL_NAME=lib$(LIB_GAME).so

# Object files for game
OBJ_CXX_GAME=$(SRC_CXX_GAME:%.cpp=$(BUILD_DIR)/%.o)
OBJ_C_GAME=$(SRC_C_GAME:%.c=$(BUILD_DIR)/%.o)
OBJ_GAME=$(OBJ_C_GAME)
OBJ_GAME+=$(OBJ_CXX_GAME)

# Object files for tests
OBJ_CXX_TEST=$(SRC_CXX_TEST:%.cpp=$(BUILD_DIR)/%.o)
OBJ_C_TEST=$(SRC_C_TEST:%.c=$(BUILD_DIR)/%.o)
OBJ_TEST=$(OBJ_C_TEST)
OBJ_TEST+=$(OBJ_CXX_TEST)
OBJ_TEST+=$(OBJ_GAME)

# Objects files for resources
SRC_RC_TMP = $(RC_ALL:=.c)
SRC_RC = $(SRC_RC_TMP:%=$(BUILD_DIR)/%)
OBJ_RC = $(SRC_RC:%.c=%.o)
OBJ_GAME += $(OBJ_RC)

# Dependencies for game
DEPS_CXX_GAME=$(OBJ_CXX_GAME:%.o=%.d)
DEPS_C_GAME=$(OBJ_C_GAME:%.o=%.d)
DEPS_GAME=$(DEPS_C_GAME)
DEPS_GAME+=$(DEPS_CXX_GAME)

DEPS_CXX_TEST=$(OBJ_CXX_TEST:%.o=%.d)
DEPS_C_TEST=$(OBJ_C_TEST:%.o=%.d)
DEPS_TEST=$(DEPS_C_TEST)
DEPS_TEST+=$(DEPS_CXX_TEST)

# APK file
APK_NAME=android-arm-debug.apk
APK_DIR=android/build/outputs/apk/arm/debug/


###
# Rules
##################

# Main build target
all: check_source_exists $(BIN)

# Just an alias for everything that needs to be rebuild
# if the Makefile changes
FORCE_REBUILD: Makefile

check_source_exists:
ifeq ($(SRC_CXX_GAME)$(SRC_C_GAME),)
	$(error "No source file. Build failed !")
endif

$(BIN): $(BUILD_DIR)/$(BIN)

$(BIN_TEST): $(BUILD_DIR)/$(BIN_TEST)

$(LIB_GAME): $(BUILD_DIR)/$(LIB_GAME_FULL_NAME)

# Final linker call
$(BUILD_DIR)/$(BIN): $(SRC_BIN) $(BUILD_DIR)/$(LIB_GAME_FULL_NAME)
	@mkdir -pv $(@D)
	$(CXX) -o $@ $(SRC_BIN) $(CXXFLAGS) -L$(BUILD_DIR) -l$(LIB_GAME) $(LDFLAGS)

$(BUILD_DIR)/$(LIB_GAME_FULL_NAME): $(OBJ_GAME)
	@mkdir -pv $(@D)
	$(CXX) -shared -o $(BUILD_DIR)/$(LIB_GAME_FULL_NAME) $^ $(LDFLAGS)

$(BUILD_DIR)/$(BIN_TEST): $(OBJ_TEST) $(BUILD_DIR)/$(LIB_GAME_FULL_NAME)
	@mkdir -pv $(@D)
	$(CXX) -o $@ $(OBJ_TEST) -L$(BUILD_DIR) -l$(LIB_GAME) $(LDFLAGS)


# Include dependencies
-include $(DEPS_GAME)
-include $(DEPS_TEST)


# General build target
$(BUILD_DIR)/%.o: %.cpp $(RC_HEADER) $(RC_CPP_INTERNAL) FORCE_REBUILD
	@mkdir -pv $(@D)
	$(CXX) -o $@ -c $< $(CXXFLAGS) -MMD

$(BUILD_DIR)/%.o: %.c $(RC_HEADER) FORCE_REBUILD
	@mkdir -pv $(@D)
	$(CC) -o $@ -c $< $(CFLAGS) -MMD

%.o: %.c FORCE_REBUILD
	@mkdir -pv $(@D)
	$(CC) -o $@ -c $< $(CFLAGS) -MMD

# Building resources
$(BUILD_DIR)/%.c : % FORCE_REBUILD
	@mkdir -pv $(@D)
	$(RC_BUILD) rc2c $< --output $@

# Building rc.h
$(RC_HEADER): $(RC_ALL) FORCE_REBUILD
	$(RC_BUILD) rc2h rc/ --output $@

# Building an internal part for ressource manager
$(RC_CPP_INTERNAL): $(RC_ALL) FORCE_REBUILD
	$(RC_BUILD) rc2cpp rc/ --output $@

# Clean-up targets
.PHONY: clean mrproper launch debug memcheck
clean:
	-rm -f $(BUILD_DIR)/$(BIN) $(BUILD_DIR)/$(BIN_TEST) \
		$(BUILD_DIR)/$(LIB_GAME_FULL_NAME) $(OBJ_GAME) $(OBJ_TEST) \
	 	$(DEPS_GAME) $(DEPS_TEST) *.gcda *.gcno

mrproper: clean
	-rm -rf $(BUILD_DIR) $(RC_HEADER) $(RC_CPP_INTERNAL)
	-cd android && make mrproper

# Launch targets
launch: all
	-LD_LIBRARY_PATH=$(BUILD_DIR) $(BUILD_DIR)/$(BIN) $(BIN_ARGS)

test: $(BIN_TEST)
	-LD_LIBRARY_PATH=$(BUILD_DIR) $(BUILD_DIR)/$(BIN_TEST) $(BIN_ARGS)

debug: all
	-LD_LIBRARY_PATH=$(BUILD_DIR) cgdb --quiet --args $(BUILD_DIR)/$(BIN) $(BIN_ARGS)

memcheck: all
	-LD_LIBRARY_PATH=$(BUILD_DIR) valgrind $(BUILD_DIR)/$(BIN)

coverage: test
	-mkdir -pv $(BUILD_DIR)/gcov
	-gcov -r $(SRC_CXX_GAME:%=$(BUILD_DIR)/%)
	-mv *.gcov $(BUILD_DIR)/gcov
	-ag "#####" $(BUILD_DIR)/gcov

apk: $(BUILD_DIR)/$(APK_NAME)

$(BUILD_DIR)/$(APK_NAME): $(APK_DIR)/$(APK_NAME)
	-cp -v $? $@

$(APK_DIR)/$(APK_NAME): $(SRC_RC) $(RC_HEADER) $(SRC_CXX_GAME)
	-cd android && make

## Makefile debug
# Keep intermediary files
.SECONDARY:
