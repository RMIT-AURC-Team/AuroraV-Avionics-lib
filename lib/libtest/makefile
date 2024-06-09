# Define project name
PROJECT_NAME = libtest

CC = gcc
BIN = bin
CFLAGS = -Wall -g
OBJDIR = build

# Define all source files
SRCS = $(shell find src/ -name "*.c") 
HDRS = $(shell find src/ -name "*.h")
LIB = $(BIN)/libtest.a

# Define object files based on source files
OBJS = $(SRCS:src/%.c=build/%.o)

# Define target (executable name)
TARGET = libtest

all: $(LIB)

$(LIB): $(OBJS)
	@mkdir -p $(@D)
	ar rcs $@ $^

# Compile source files into object files
$(OBJDIR)/%.o: src/%.c $(HDRS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ 

# Define clean target to remove object files and executable
clean:
	@rm -rf $(OBJDIR)/* $(BIN)

.PHONY: all clean test

