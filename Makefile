# compiler
CC = g++
CXXFLAGS = -Wall -Wextra

# application configs
APP_NAME=server

# libraries
LIB = -lws2_32

# directories
BIN_DIR = bin
SRC_DIR = src
OBJ_DIR = $(BIN_DIR)/obj

# source files
SRC  			= $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp) $(wildcard $(SRC_DIR)/**/**/*.cpp)
OBJ  			= $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))
BUILD_APP = $(BIN_DIR)/$(APP_NAME)

# mkdir utility
MKDIR = mkdir -p $(1)
ifeq ($(OS),Windows_NT)
	MKDIR = if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
endif

build: $(OBJ)
	$(call MKDIR,$(dir $(BUILD_APP)))
	$(CC) -o $(BUILD_APP) $^ $(LIB)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(call MKDIR,$(dir $@))
	$(CC) $(CXXFLAGS) -o $@ -c $<

server:
	./$(BUILD_APP)

# rm utility
ifeq ($(OS),Windows_NT)
    RM_FILE := cmd /C erase /Q /F
    RM_DIR  := cmd /C rmdir /S /Q
    BUILD_APP_Q := $(subst /,\,$(BUILD_APP).exe)
    OBJ_DIR_Q   := $(subst /,\,$(OBJ_DIR))
else
    RM_FILE := rm -f
    RM_DIR  := rm -rf
endif

.PHONY: clean

clean:
ifeq ($(OS),Windows_NT)
	if exist "$(BUILD_APP_Q)" $(RM_FILE) "$(BUILD_APP_Q)"
	if exist "$(OBJ_DIR_Q)"   $(RM_DIR)  "$(OBJ_DIR_Q)"
else
	$(RM_FILE) "$(BUILD_APP)"
	$(RM_DIR)  "$(OBJ_DIR)"
endif