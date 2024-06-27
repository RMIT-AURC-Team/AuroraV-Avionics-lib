SUBPROJECTS = membuff quaternion kalmanfilter
ROOT = $(shell pwd)
BIN = bin
INC = inc
# Normalise path if on windows
ifeq ($(OS),Windows_NT)
	ROOT := $(shell cygpath -m $(ROOT))
endif

LIBTEST_DIR = $(ROOT)/lib/libtest
LIBTEST_LIB = $(LIBTEST_DIR)/bin/libtest.a

LIBCMSIS_DIR = $(ROOT)/lib/libcmsis
LIBCMSIS_LIB = $(LIBCMSIS_DIR)/bin/libCMSISDSP.a

ifdef TOOLCHAIN

# --- ARMCC ---
ifeq ($(TOOLCHAIN),armcc)
CC = armcc
AR = armar
CFLAGS = --cpu cortex-m4 	\
				 --fpu fpv4-sp 		\
				 -O3
LIBTEST_ARG = -L--userlibpath=$(LIBTEST_DIR)/bin -L--lib=test -I$(LIBTEST_DIR)/src
LIBCMSIS_ARG = -L--userlibpath=$(LIBCMSIS_DIR)/bin -L--lib=CMSISDSP

# --- ARMCLANG ---
else ifeq ($(TOOLCHAIN),armclang)
CC = armclang
AR = armar
CFLAGS = -mcpu=cortex-m4 		 	 \
         -mfloat-abi=hard  		 \
         -mfpu=fpv4-sp-d16 		 \
         -Ofast -ffast-math 	 \
         -DNDEBUG 						 \
         -Wall -Wextra 				 \
         --target=arm-arm-none-eabi
LIBTEST_ARG = -L$(LIBTEST_DIR)/bin -ltest -I$(LIBTEST_DIR)/src
LIBCMSIS_ARG = -L$(LIBCMSIS_DIR)/bin -lCMSISDSP

# --- ARMGCC ---
else ifeq ($(TOOLCHAIN),armgcc)
CC = arm-none-eabi-gcc
AR = arm-none-eabi-ar
CFLAGS = -mcpu=cortex-m4 		 	 \
         -mfloat-abi=hard  		 \
         -mfpu=fpv4-sp-d16 		 \
         -Ofast -ffast-math 	 \
         -DNDEBUG 						 \
         -Wall -Wextra -Werror \
LIBTEST_ARG = -L$(LIBTEST_DIR)/bin -ltest -I$(LIBTEST_DIR)/src
LIBCMSIS_ARG = -L$(LIBCMSIS_DIR)/bin -lCMSISDSP

# --- GCC ---
else
CC = gcc
AR = ar
CFLAGS := -Wall -g
LIBTEST_ARG = -L$(LIBTEST_DIR)/bin -ltest -I$(LIBTEST_DIR)/src
LIBCMSIS_ARG = -L$(LIBCMSIS_DIR)/bin -lCMSISDSP
endif

endif

# Includes for CMSIS lib
CMSIS_ROOT = $(LIBCMSIS_DIR)/src
CMSIS_5 := $(CMSIS_ROOT)/CMSIS_5
CMSIS_DSP := $(CMSIS_ROOT)/CMSIS-DSP

CMSIS_CORE_INCLUDES := $(CMSIS_5)/CMSIS/Core/Include 

DSP_INCLUDES := $(CMSIS_DSP)/Include \
								$(CMSIS_DSP)/PrivateInclude 

CMSIS_INC := $(LIBCMSIS_ARG)
CMSIS_INC += $(addprefix -I,$(DSP_INCLUDES))
CMSIS_INC += $(addprefix -I,$(CMSIS_CORE_INCLUDES))

# Define the static library targets for each subproject
SUBPROJECT_LIBS := $(foreach proj,$(SUBPROJECTS),$(proj)/bin/lib$(proj).a)
COMBINED_LIB = $(BIN)/libavionics.a

# Default target
.PHONY: all subprojects test clean

all: $(COMBINED_LIB)

test: $(LIBCMSIS_LIB) $(LIBTEST_LIB)
	@echo "$(CFLAGS)"
	@echo "Running tests..."
	@TOTAL_TESTS_RUN=0; TOTAL_TESTS_PASSED=0; 																		 \
	for proj in $(SUBPROJECTS); do 																								 \
	    TEST_OUTPUT=$$(make -C $$proj test LIBTEST_ARG="$(LIBTEST_ARG)" 					 \
																				 LIBTEST_DIR="$(LIBTEST_DIR)"					 	 \
																				 CMSIS_INC="$(CMSIS_INC)"					 			 \
																				 AR="$(AR)" 												     \
																				 CC="$(CC)" CFLAGS:="$(CFLAGS) -fPIC");	 \
	    echo "$$TEST_OUTPUT"; 																										 \
	    TESTS_RUN=$$(echo "$$TEST_OUTPUT" | grep -oP '(?<=number of tests: )\d+'); \
	    TESTS_PASSED=$$(echo "$$TEST_OUTPUT" | grep -oP '(?<=tests passed: )\d+'); \
	    TOTAL_TESTS_RUN=$$((TOTAL_TESTS_RUN + TESTS_RUN)); 												 \
	    TOTAL_TESTS_PASSED=$$((TOTAL_TESTS_PASSED + TESTS_PASSED)); 							 \
	done; 																					 															 \
	printf "\n"; 																		 															 \
	echo "TEST SUMMARY"; 														 															 \
	echo "=========================="; 							 															 \
	if [ $$TOTAL_TESTS_RUN == $$TOTAL_TESTS_PASSED ]; then 												 \
		echo "ALL TESTS PASSED"; 																										 \
	fi; 	 																																				 \
	echo "Total number of tests: $$TOTAL_TESTS_RUN"; 															 \
	echo "Total tests passed: $$TOTAL_TESTS_PASSED"; 															 \
	echo "=========================="; 

clean: clean-libtest clean-libcmsis $(patsubst %,clean-%,$(SUBPROJECTS))
	rm -rf $(BIN)/* $(INC)/*

# Special rule for building subprojects
subprojects:
	@mkdir -p $(INC)
	@for proj in $(SUBPROJECTS); do 														\
		make -C $$proj CC="$(CC)"  AR="$(AR)" CFLAGS="$(CFLAGS)" 	\
									 						 LIBCMSIS_ARG="$(LIBCMSIS_ARG)" \
									 						 LIBCMSIS_DIR="$(LIBCMSIS_DIR)" \
									 						 CMSIS_INC="$(CMSIS_INC)"; 			\
		ln -rsf $$proj/src/$$proj.h $(INC)/$$proj.h; 							\
	done;

# Combine static subproject libraries to single combined library
# and link includes
$(COMBINED_LIB): $(LIBCMSIS_LIB) subprojects
	@mkdir -p $(@D)
	@mkdir -p $(INC)/CORE
	@mkdir -p $(INC)/DSP
	$(AR) -crs $@ $(SUBPROJECT_LIBS) $(LIBCMSIS_LIB)
	@for path in $(DSP_INCLUDES); do  		\
		DIR=basename $$path; 				  			\
		ln -rsf $$path $(INC)/DSP/$$DIR; 		\
	done;
	ln -rsf $(CMSIS_CORE_INCLUDES) $(INC)/CORE/$(notdir $(CMSIS_CORE_INCLUDES))


$(LIBTEST_LIB):
	@$(MAKE) -C $(LIBTEST_DIR) CC="$(CC)"  AR="$(AR)" CFLAGS="$(CFLAGS) -fPIC"

$(LIBCMSIS_LIB):
	@$(MAKE) -C $(LIBCMSIS_DIR) CC="$(CC)"  AR="$(AR)" CFLAGS="$(CFLAGS)"

# Pattern rule for testing subprojects
test-%: $(LIBTEST_LIB) $(LIBCMSIS_LIB)
	@$(MAKE) -C $* test LIBTEST_ARG="$(LIBTEST_ARG)" LIBTEST_DIR="$(LIBTEST_DIR)" 		\
											LIBCMSIS_ARG="$(LIBCMSIS_ARG)" LIBCMSIS_DIR="$(LIBCMSIS_DIR)" \

clean-%:
	@$(MAKE) -C $* clean

clean-libtest:
	@$(MAKE) -C $(LIBTEST_DIR) clean

clean-libcmsis:
	@$(MAKE) -C $(LIBCMSIS_DIR) clean
