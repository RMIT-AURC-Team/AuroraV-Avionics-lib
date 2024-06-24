ROOT = $(shell pwd)
BIN = bin
SUBPROJECTS = membuff quaternion kalmanfilter

LIBTEST_DIR = $(ROOT)/lib/libtest
LIBTEST_LIB = $(LIBTEST_DIR)/bin/libtest.a
LIBTEST_ARG = -L$(LIBTEST_DIR)/bin -ltest -I$(LIBTEST_DIR)/src

LIBCMSIS_DIR = $(ROOT)/lib/libcmsis
LIBCMSIS_LIB = $(LIBCMSIS_DIR)/bin/libCMSISDSP.a
LIBCMSIS_ARG = -L$(LIBCMSIS_DIR)/bin -lCMSISDSP

# CC = arm-none-eabi-gcc
# AR = arm-none-eabi-ar
# CFLAGS = -mcpu=cortex-m4 			 \
         -mfloat-abi=hard  		 \
         -mfpu=fpv4-sp-d16 		 \
         -Ofast -ffast-math 	 \
         -DNDEBUG 						 \
         -Wall -Wextra -Werror \

CC = gcc
AR = ar
CFLAGS = -Wall -g

ifdef PIC
	CFLAGS += -fPIC
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

test: $(LIBTEST_LIB)
	@echo "Running tests..."
	@TOTAL_TESTS_RUN=0; TOTAL_TESTS_PASSED=0; 																		 \
	for proj in $(SUBPROJECTS); do 																								 \
	    TEST_OUTPUT=$$(make -C $$proj test LIBTEST_ARG="$(LIBTEST_ARG)" 					 \
																				 LIBTEST_DIR="$(LIBTEST_DIR)"					 	 \
																				 LIBCMSIS_ARG="$(LIBCMSIS_ARG)"					 \
																				 LIBCMSIS_DIR="$(LIBCMSIS_DIR)" 				 \
																				 AR="$(AR)" 												     \
																				 CC="$(CC)" CFLAGS="$(CFLAGS)");				 \
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
	rm -rf $(BIN)/*

# Special rule for building subprojects
subprojects:
	@for proj in $(SUBPROJECTS); do 														\
		make -C $$proj CC="$(CC)"  AR="$(AR)" CFLAGS="$(CFLAGS)" 	\
									 LIBCMSIS_ARG="$(LIBCMSIS_ARG)"  						\
									 LIBCMSIS_DIR="$(LIBCMSIS_DIR)"  						\
									 CMSIS_INC="$(CMSIS_INC)" 			 						\
									 PIC=$(PIC);										 						\
	done;

# Combine static subproject libraries to single combined library
$(COMBINED_LIB): $(LIBCMSIS_LIB) subprojects
	@mkdir -p $(@D)
	$(AR) -crs $@ $(SUBPROJECT_LIBS) $(LIBCMSIS_LIB)

$(LIBTEST_LIB):
	@$(MAKE) -C $(LIBTEST_DIR)

$(LIBCMSIS_LIB):
	@$(MAKE) -C $(LIBCMSIS_DIR) CC="$(CC)"  AR="$(AR)" CFLAGS="$(CFLAGS)"

# Pattern rule for testing subprojects
test-%: $(LIBTEST_LIB) $(LIBCMSIS_LIB)
	@$(MAKE) -C $* test LIBTEST_ARG="$(LIBTEST_ARG)" LIBTEST_DIR="$(LIBTEST_DIR)" 		\
											LIBCMSIS_ARG="$(LIBCMSIS_ARG)" LIBCMSIS_DIR="$(LIBCMSIS_DIR)" \
											PIC=$(PIC)

clean-%:
	@$(MAKE) -C $* clean

clean-libtest:
	@$(MAKE) -C $(LIBTEST_DIR) clean

clean-libcmsis:
	@$(MAKE) -C $(LIBCMSIS_DIR) clean
