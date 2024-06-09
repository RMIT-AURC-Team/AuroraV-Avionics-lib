# Define project name
PROJECT_NAME = membuff

CC = gcc
CFLAGS = -Wall -g
OBJDIR = build

# Define all source files
SRCS = $(shell find src/ -name "*.c") 
HDRS = $(shell find src/ -name "*.h")
TSTS = $(shell find test/ -name "*.c")

# Define object files based on source files
OBJS = $(SRCS:src/%.c=build/%.o)
TEST_OBJS = $(TSTS:test/%.c=build/%.o)

# Define target (executable name)
TARGET = $(OBJDIR)/membuff
TEST_TARGET = $(OBJDIR)/membuff_test

all: $(TARGET)

# Run unit tests
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Compile source files into object files
$(OBJDIR)/%.o: src/%.c $(HDRS)
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ 

# Compile test files into object files
$(OBJDIR)/%.o: test/%.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@ -I src/

# Link object files into executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -shared $(OBJS) -o $@.a

# Compile test source for test target
$(TEST_TARGET): $(OBJS) $(TEST_OBJS) $(HDRS)
	$(CC) $(CFLAGS) $(OBJS) $(TEST_OBJS) -o $@

# Define clean target to remove object files and executable
clean:
	rm $(OBJDIR)/*

.PHONY: all clean test
