# Compiler and flags
CC := gcc
INCLUDES := -I./includes
CFLAGS := -Wall -Wextra -O3 $(INCLUDES)

# Directories
LIBS_DIR := libs
EXAMPLES_DIR := examples
OBJECTS_DIR := objects
BIN_DIR := bin

# Find all source files
LIB_SOURCES := $(shell find $(LIBS_DIR) -name '*.c')
EXAMPLES := $(shell find $(EXAMPLES_DIR) -name '*.c')

# Map library sources to object files (preserve structure under objects/)
LIB_OBJECTS := $(patsubst $(LIBS_DIR)/%.c, $(OBJECTS_DIR)/%.o, $(LIB_SOURCES))

# Output executables (one per example source)
EXECS := $(patsubst $(EXAMPLES_DIR)/%.c, $(BIN_DIR)/%, $(EXAMPLES))

# Default target
all: $(LIB_OBJECTS) $(EXECS)

# Rule to build each example into an executable
$(BIN_DIR)/%: $(EXAMPLES_DIR)/%.c $(LIB_OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< $(LIB_OBJECTS) -o $@
	@echo "Built executable: $@"

# Rule to compile library source files into objects
$(OBJECTS_DIR)/%.o: $(LIBS_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled: $< → $@"

# Clean up all build artifacts
clean:
	rm -rf $(OBJECTS_DIR) $(BIN_DIR)
	@echo "Clean complete."

.PHONY: all clean
