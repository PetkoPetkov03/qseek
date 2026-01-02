CC  := gcc
CXX := g++

INCLUDES := -I./includes
CFLAGS   := -g -Wall -Wextra -Wshadow -Wuninitialized -O0 $(INCLUDES) -lm
CXXFLAGS := -g -Wall -Wextra -Wshadow -O0 $(INCLUDES)

# Directories
LIBS_DIR     := libs
EXAMPLES_DIR := examples
OBJECTS_DIR  := objects
BIN_DIR      := bin

# Find source files
LIB_C_SOURCES   := $(shell find $(LIBS_DIR) -name '*.c')
LIB_CPP_SOURCES := $(shell find $(LIBS_DIR) -name '*.cc')
EXAMPLES        := $(shell find $(EXAMPLES_DIR) -name '*.c')

# Map sources to objects
LIB_C_OBJECTS   := $(patsubst $(LIBS_DIR)/%.c,   $(OBJECTS_DIR)/%.o, $(LIB_C_SOURCES))
LIB_CPP_OBJECTS := $(patsubst $(LIBS_DIR)/%.cc,  $(OBJECTS_DIR)/%.o, $(LIB_CPP_SOURCES))
LIB_OBJECTS     := $(LIB_C_OBJECTS) $(LIB_CPP_OBJECTS)

# Output executables
EXECS := $(patsubst $(EXAMPLES_DIR)/%.c, $(BIN_DIR)/%, $(EXAMPLES))

# Default target
all: $(LIB_OBJECTS) $(EXECS)

# Link executables (always use g++)
$(BIN_DIR)/%: $(EXAMPLES_DIR)/%.c $(LIB_OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< $(LIB_OBJECTS) -o $@
	@echo "Built executable: $@"

# Compile C files with gcc
$(OBJECTS_DIR)/%.o: $(LIBS_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled C: $< → $@"

# Compile C++ files with g++
$(OBJECTS_DIR)/%.o: $(LIBS_DIR)/%.cc
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled C++: $< → $@"

# Clean
clean:
	rm -rf $(OBJECTS_DIR) $(BIN_DIR)
	@echo "Clean complete."

.PHONY: all clean
