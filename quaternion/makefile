# Define project name
PROJECT_NAME = libquaternion

BIN = bin
OBJDIR = build

# Define all source files
SRCS = $(wildcard src/*.c) 
HDRS = $(wildcard src/*.h)
TSTS = $(wildcard test/*.c)
PYTSTS = $(wildcard test/*.py)

# Define object files based on source files
OBJS = $(SRCS:src/%.c=build/%.o)
TEST_OBJS = $(TSTS:test/%.c=build/%.o)

# Define target (executable name)
TARGET = $(BIN)/$(PROJECT_NAME).a
SHARED = $(BIN)/$(PROJECT_NAME).so
TEST_TARGET = $(BIN)/$(PROJECT_NAME)_test

ifdef PIC
all: $(SHARED)
else
all: $(TARGET)
endif

# Run unit tests
test: $(TEST_TARGET) $(SHARED)
	./$(TEST_TARGET)
	@for test in $(PYTSTS); do \
		python $$test; 					 \
	done;

# Link object files into executable
$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(AR) -rcs $@ $^

# Link object files into shared library
$(SHARED): $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -shared -o $@ $^ $(CMSIS_INC)

# Compile source files into object files
$(OBJDIR)/%.o: src/%.c $(HDRS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile test source for test target
$(TEST_TARGET): $(OBJS) $(TEST_OBJS) $(HDRS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(OBJS) $(TEST_OBJS) -o $@ $(LIBTEST_ARG) -lm

# Compile test files into object files
$(OBJDIR)/%.o: test/%.c $(HDRS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBTEST_ARG) -I src/ -I$(LIBTEST_DIR)

# Define clean target to remove object files and executable
clean:
	rm -rf $(OBJDIR)/* $(BIN)/*

.PHONY: all clean test
