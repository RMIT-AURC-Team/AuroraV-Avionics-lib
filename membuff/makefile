PROJECT_NAME = membuff
CC = gcc
CFLAGS = -Wall -g
OBJDIR = build

# Define all source files
SRCS = $(shell find src/ -name "*.c")
HDRS = $(shell find src/ -name "*.h")

OBJS = $(SRCS:membuff/%.c=build/%.o)
TARGET = membuff
TEST_TARGET = $(TARGET)_test

all: $(TARGET)

# Compile source files into object files
$(OBJDIR)/%.o: membuff/%.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

# Compile test source for test target
$(TEST_TARGET): $(OBJDIR)/test_membuff.o $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(OBJDIR)/test_membuff.o -o $@ -lcunit

# Define clean target to remove object files and executable
clean:
	rm -rf $(OBJDIR)/* 

# Run unit tests
test: $(TEST_TARGET)
	./$(TEST_TARGET)

.PHONY: all clean test
