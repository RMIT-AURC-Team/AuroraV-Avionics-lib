# Define project name
PROJECT_NAME = libtest

CC = gcc
CFLAGS = -Wall -g
BUILD_DIR = build

# Define all source files
SRCS = $(shell find src/ -name "*.c") 
HDRS = $(shell find src/ -name "*.h")
LIB = $(BUILD_DIR)/libtest.a

# Define object files based on source files
OBJS = $(SRCS:src/%.c=build/%.o)

# Define target (executable name)
TARGET = libtest

all: $(LIB)

$(LIB): $(OBJS)
	ar rcs $@ $^

# Compile source files into object files
$(BUILD_DIR)/%.o: src/%.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@ 

# Define clean target to remove object files and executable
clean:
	@rm -rf $(BUILD_DIR)/*

.PHONY: all clean test

