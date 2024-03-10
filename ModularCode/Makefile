# Makefile

# Compiler
CXX := g++

# Include directory
INCLUDE_DIR := include

# Source directory
SRC_DIR := src

# Objects directory
OBJ_DIR := obj

# Binary directory
BIN_DIR := bin

# Compiler flags
CXXFLAGS := -std=c++11 -I$(INCLUDE_DIR) `pkg-config --cflags opencv4`

# Linker flags
LDFLAGS := `pkg-config --libs opencv4` -lpthread


# Source files
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJECTS := $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Binary target
TARGET := $(BIN_DIR)/myApplication

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR)

