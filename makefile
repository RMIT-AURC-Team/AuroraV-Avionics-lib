ROOT = $(shell pwd)
BIN = bin
SUBPROJECTS = quaternion
LIBTEST_DIR = $(ROOT)/lib/libtest
LIBTEST_LIB = $(LIBTEST_DIR)/bin/libtest.a
LIBTEST_ARG = -L$(LIBTEST_DIR)/bin -ltest -I$(LIBTEST_DIR)/src

CC = gcc
CFLAGS = -Wall -g

# Define the shared library targets for each subproject
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
																				 LIBTEST_DIR="$(LIBTEST_DIR)");				 	 \
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

clean: clean-libtest $(patsubst %,clean-%,$(SUBPROJECTS))
	rm -rf $(BIN)/*

# Combine static subproject libraries to single combined library
$(COMBINED_LIB): subprojects
	@mkdir -p $(@D)
	$(AR) -crs $@ $(SUBPROJECT_LIBS)

# Special rule for building subprojects
subprojects:
	$(MAKE) -C $(SUBPROJECTS)

# Special rule for building libtest
$(LIBTEST_LIB):
	@$(MAKE) -C $(LIBTEST_DIR)

# Pattern rule for testing subprojects
test-%: $(LIBTEST_LIB)
	@$(MAKE) -C $* test LIBTEST_ARG="$(LIBTEST_ARG)" LIBTEST_DIR="$(LIBTEST_DIR)"

# Pattern rule for cleaning subprojects
clean-%:
	@$(MAKE) -C $* clean

# Special rule for cleaning libtest
clean-libtest:
	@$(MAKE) -C $(LIBTEST_DIR) clean
