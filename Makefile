# Makefile for Error-Friendly Compiler

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
LDFLAGS = 

# Directories
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
TARGET = $(BIN_DIR)/compiler

# Default target
all: $(TARGET)

# Create directories if they don't exist
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Link the executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	@echo "Linking $(TARGET)..."
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $(TARGET)
	@echo "Build complete! Executable: $(TARGET)"

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Clean complete!"

# Run the compiler with sample input
run: $(TARGET)
	@echo "Running compiler..."
	$(TARGET)

# Run tests
test: $(TARGET)
	@echo "=== Testing Valid Program ==="
	$(TARGET) tests/valid_program.ef
	@echo ""
	@echo "=== Testing Lexical Errors ==="
	$(TARGET) tests/lexical_errors.ef
	@echo ""
	@echo "=== Testing Syntax Errors ==="
	$(TARGET) tests/syntax_errors.ef
	@echo ""
	@echo "=== Testing Semantic Errors ==="
	$(TARGET) tests/semantic_errors.ef

.PHONY: all clean run test
