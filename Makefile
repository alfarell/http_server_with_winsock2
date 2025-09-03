# compiler
CC = g++

# application configs
APP_NAME=server

# libraries
LIB = -lws2_32

# directories
BIN_DIR = bin
SRC_DIR = src
OBJ_DIR = obj

# source files
SRC  			= $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp) $(wildcard $(SRC_DIR)/**/**/*.cpp)
OBJ  			= $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))
BUILD_APP = $(BIN_DIR)/$(APP_NAME)

# windows utility
MKDIR = if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
RM_FILE = del /Q /F
BUILD_APP_Q = $(subst /,\,$(BIN_DIR))
OBJ_DIR_Q   = $(subst /,\,$(OBJ_DIR))

.PHONY: clean

build: $(OBJ)
	$(call MKDIR,$(dir $(BUILD_APP)))
	$(CC) -o $(BUILD_APP) $^ $(LIB)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(call MKDIR,$(dir $@))
	$(CC) -o $@ -c $<

run-server:
	./$(BUILD_APP)

clean:
	$(RM_FILE) "$(BUILD_APP_Q)" "$(OBJ_DIR_Q)"
