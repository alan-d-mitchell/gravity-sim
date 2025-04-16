# Makefile for Advanced Gravity Simulation

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11
LDFLAGS = -lGL -lGLEW -lglfw -lm

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Find all .c files
SRCS := $(shell find $(SRC_DIR) -name "*.c")
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# Set target name
TARGET = $(BIN_DIR)/gravity_sim

# Create directory structure
DIRS := $(sort $(dir $(OBJS)) $(BIN_DIR))

# Main targets
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS) | $(BIN_DIR)
	@echo "Linking $@"
	@$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(DIRS)
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Create directories
$(DIRS):
	@mkdir -p $@

# Include dependency files if they exist
-include $(DEPS)

# Clean built files
clean:
	@echo "Cleaning build files"
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

# Run the simulation
run: all
	@echo "Running simulation"
	@$(TARGET)

# Help target
help:
	@echo "Gravity Simulation Makefile"
	@echo "Targets:"
	@echo "  all        - Build the simulation"
	@echo "  clean      - Remove build files"
	@echo "  run        - Build and run the simulation"
	@echo "  help       - Display this help"

.PHONY: all clean run help