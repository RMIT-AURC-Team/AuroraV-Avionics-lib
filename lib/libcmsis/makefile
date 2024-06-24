
CMSIS_ROOT := src

CMSIS_5 := $(CMSIS_ROOT)/CMSIS_5
CMSIS_DSP := $(CMSIS_ROOT)/CMSIS-DSP

# Includes
CMSIS_CORE_INCLUDES := $(CMSIS_5)/CMSIS/Core/Include
DSP_INCLUDES := $(CMSIS_DSP)/Include \
                $(CMSIS_DSP)/PrivateInclude

# Compilation flags for include folders
INC_FLAGS := $(addprefix -I,$(DSP_INCLUDES))
INC_FLAGS += $(addprefix -I,$(CMSIS_CORE_INCLUDES))

# Sources
SRCS := $(CMSIS_DSP)/Source/BasicMathFunctions/BasicMathFunctions.c \
        $(CMSIS_DSP)/Source/SupportFunctions/arm_copy_f32.c         \
        $(CMSIS_DSP)/Source/CommonTables/CommonTables.c             \
        $(CMSIS_DSP)/Source/MatrixFunctions/MatrixFunctions.c       \

# Output folder for build products
OBJDIR := build
BIN := bin

# Object files list
OBJECTS := $(SRCS:%.c=$(OBJDIR)/%.o)

# Static and shared libraries
STATIC_LIB := $(BIN)/libCMSISDSP.a
SHARED_LIB := $(BIN)/libCMSISDSP.so

# Build all targets
#
all: $(STATIC_LIB) 

# Build static library
$(STATIC_LIB): $(OBJECTS)
	$(AR) -rcs $@ $^

# Build shared library
$(SHARED_LIB): $(OBJECTS)
	$(CC) -shared $^ $(CFLAGS) $(INC_FLAGS) -o $@

# Compile each source file to object file
$(OBJDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $(INC_FLAGS) $< -o $@

# Clean build artifacts
clean:
	rm -rf $(OBJDIR)/* $(BIN)/*

.PHONY: all clean
