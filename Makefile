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
			 -Wconversion -Wfloat-equal -Wundef -Wpointer-arithm -Wcast-align \
			 -Wunreachable-code -fPIC
CXXFLAGS=-std=c++11 -g -Wall -Wextra -pedantic -Wshadow -Weffc++ \
				 -Wunreachable-code -Wconversion -fPIC

# Linker flags
LDFLAGS=-lsfml-system -lsfml-window -lsfml-graphics

# Source files (adjust if needed)
SRC_CXX_GAME=$(wildcard game/*.cpp)
SRC_C_GAME=$(wildcard game/*.c)
SRC_CXX_TEST=$(wildcard tests/*.cpp)
SRC_C_TEST=$(wildcard tests/*.c)
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

# Dependencies for game
DEPS_CXX_GAME=$(OBJ_CXX_GAME:%.o=%.d)
DEPS_C_GAME=$(OBJ_C_GAME:%.o=%.d)
DEPS_GAME=$(DEPS_C_GAME)
DEPS_GAME+=$(DEPS_CXX_GAME)

DEPS_CXX_TEST=$(OBJ_CXX_TEST:%.o=%.d)
DEPS_C_TEST=$(OBJ_C_TEST:%.o=%.d)
DEPS_TEST=$(DEPS_C_TEST)
DEPS_TEST+=$(DEPS_CXX_TEST)

###
# Rules
##################

# Main build target
all: check_source_exists $(BIN)

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
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -pv $(@D)
	$(CXX) -o $@ -c $< $(CXXFLAGS) -MMD

$(BUILD_DIR)/%.o: %.c
	@mkdir -pv $(@D)
	$(CC) -o $@ -c $< $(CFLAGS) -MMD


# Clean-up targets
.PHONY: clean mrproper launch debug
clean:
	-rm -f $(BUILD_DIR)/$(BIN) $(BUILD_DIR)/$(BIN_TEST) \
		$(BUILD_DIR)/$(LIB_GAME_FULL_NAME) $(OBJ_GAME) $(OBJ_TEST) \
	 	$(DEPS_GAME) $(DEPS_TEST)

mrproper: clean
	-rm -rf $(BUILD_DIR)

# Launch targets
launch: all
	-LD_LIBRARY_PATH=$(BUILD_DIR) $(BUILD_DIR)/$(BIN) $(BIN_ARGS)

test: $(BIN_TEST)
	-LD_LIBRARY_PATH=$(BUILD_DIR) $(BUILD_DIR)/$(BIN_TEST) $(BIN_ARGS)

debug: all
	-LD_LIBRARY_PATH=$(BUILD_DIR) cgdb --quiet --args $(BUILD_DIR)/$(BIN) $(BIN_ARGS)
