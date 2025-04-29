# Compiler and flags
CC = g++
CFLAGS = -Wall -Wextra -std=c++11 -Wno-unknown-pragmas -Wno-unused-parameter -Wno-unused-value
INCLUDE = -Isrc/include
LIBS = -lncurses

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = test

# Source and object files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Main executable name
TARGET = $(BIN_DIR)/game

# Test files
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJECTS = $(patsubst $(TEST_DIR)/%.cpp,$(OBJ_DIR)/test_%.o,$(TEST_SOURCES))
TEST_TARGET = $(BIN_DIR)/test

# Default target
all: $(TARGET)

# Create directories if they don't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Compile test files to object files
$(OBJ_DIR)/test_%.o: $(TEST_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Link objects to create main executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LIBS)

# Build test executable
# test: $(TEST_OBJECTS) $(filter-out $(OBJ_DIR)/main.o, $(OBJECTS)) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $(TEST_TARGET) $(LIBS)

test: $(TEST_OBJECTS) $(filter-out $(OBJ_DIR)/main.o, $(OBJECTS)) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $(TEST_TARGET) $(LIBS)

# Clean project
clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*

# Run the compiled program
run: $(TARGET)
	$(TARGET)

# Run the test program
runtest: test
	$(TEST_TARGET)

# Phony targets
.PHONY: all clean run test runtest
