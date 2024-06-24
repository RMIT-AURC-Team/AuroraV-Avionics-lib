# Define project name
PROJECT_NAME = libkalmanfilter

BIN = bin
OBJDIR = build

# Define all source files
SRCS = $(shell find src/ -name "*.c") 
HDRS = $(shell find src/ -name "*.h")
TSTS = $(shell find test/ -name "test_*.py")

# Define object files based on source files
OBJS = $(SRCS:src/%.c=build/%.o)

# Define target (executable name)
TARGET = $(BIN)/$(PROJECT_NAME).a
SHARED = $(BIN)/$(PROJECT_NAME).so

all: $(TARGET) $(SHARED)

# Define test target to run all test_*.py files in the test/ directory
test: $(TARGET)
	python $(TSTS)

# Link object files into static library
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
	$(CC) $(CFLAGS) -c -fPIC $(CMSIS_INC) $< -o $@

# Define clean target to remove object files and executable
clean:
	rm $(OBJDIR)/* $(BIN)/*

.PHONY: all clean test
