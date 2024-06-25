# Define project name
PROJECT_NAME = libmembuff

BIN = bin
OBJDIR = build

# Define all source files
SRCS = $(wildcard src/*.c) 
HDRS = $(wildcard src/*.h)
TSTS = $(wildcard test/*.c)

# Define object files based on source files
OBJS = $(SRCS:src/%.c=build/%.o)
TEST_OBJS = $(TSTS:test/%.c=build/%.o)

# Define target (executable name)
TARGET = $(BIN)/$(PROJECT_NAME).a
TEST_TARGET = $(BIN)/$(PROJECT_NAME)_test

all: $(TARGET)

# Run unit tests
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Link object files into executable
$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(AR) -rcs $@ $^

# Compile source files into object files
$(OBJDIR)/%.o: src/%.c $(HDRS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ 

# Compile test source for test target
$(TEST_TARGET): $(OBJS) $(TEST_OBJS) $(HDRS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(OBJS) $(TEST_OBJS) -o $@ $(LIBTEST_ARG)

# Compile test files into object files
$(OBJDIR)/%.o: test/%.c $(HDRS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBTEST_ARG) -I src/ -I$(LIBTEST_DIR)

# Define clean target to remove object files and executable
clean:
	rm -rf $(OBJDIR)/* $(BIN)/*

.PHONY: all clean test
