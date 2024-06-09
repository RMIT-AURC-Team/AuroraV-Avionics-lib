# Paths
SRC_DIR = src
TEST_DIR = test
BUILD_DIR = build

# Compiler and flags
CC = gcc
CFLAGS = -fPIC -Wall -Werror -g -I$(LIBTEST_DIR)/src

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.c)
HDRS = $(wildcard $(SRC_DIR)/*.h)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Test source and object files
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/%.o,$(TEST_SRCS))

TARGET = $(BUILD_DIR)/membuff.so 	# Shared library target
TEST_EXEC = membuff_test 					# Test executable

# Default target
.PHONY: all clean test

all: $(TARGET)

# Rule to create shared library
$(TARGET): $(OBJS)
	$(CC) -shared -o $@ $^

# Rule to compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HDRS)
	$(CC) $(CFLAGS) -c -o $@ $<

# Rule to compile test source files into object files
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBTEST_ARG) -Isrc/ -I$(LIBTEST_DIR)

# Rule to link the test executable
$(TEST_EXEC): $(OBJS) $(TEST_OBJS) $(LIBTEST_LIB)
	$(CC) $(CFLAGS) $(OBJS) $(TEST_OBJS) -o $@ $(LIBTEST_ARG)

# Test target
test: $(TEST_EXEC)
	@echo "Running $@..."
	@./$(TEST_EXEC)

# Clean target
clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/*.so $(TEST_EXEC)
