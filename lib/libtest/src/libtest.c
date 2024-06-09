#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include "libtest.h"

bool assert(char* test, bool assertion, bool value) {
  if (test != NULL)
    printf("Asserting %s\n", test);
  return assertion == value;
}

bool assert_buffer_equals(char* test, uint8_t* buff, uint8_t* goalBuff, int buffSize) {
  if (test != NULL)
    printf("Asserting %s\n", test);

  printf("Test buffer:\t ");
  for(int i = 0; i < buffSize; i++)
    printf("%d ", buff[i]);
  printf("\n");

  printf("Goal buffer:\t ");
  for(int i = 0; i < buffSize; i++)
    printf("%d ", goalBuff[i]);
  printf("\n\n");

  return memcmp(buff, goalBuff, buffSize) ? 0 : 1; // memcmp returns 0 if equal
}

bool assert_pointer_equals(char* test, uint8_t* point, uint8_t* goalPoint) {
  if (test != NULL)
    printf("Asserting %s\n", test);

  printf("Test pointer:\t%p\n", point);
  printf("Goal pointer:\t%p\n\n", goalPoint);

  return point == goalPoint;
}

bool assert_float_equals(char* test, float value, float goal) {
  if (test != NULL)
    printf("Asserting %s\n", test);

  printf("Test float:\t%f\n", value);
  printf("Goal loat:\t%f\n\n", goal);

  return value == goal;
}

void run_tests(int numTests, TestFunction tests[]) {
  int passCount = 0;
  bool result = false;

  for(int i = 0; i < numTests; i++) {
    printf("--------------------------------------\n\n");
    result = tests[i]();
    printf("Test %s\n\n", result ? "passed" : "failed");
    passCount += result ? 1 : 0;
  }
  printf("--------------------------------------\n");
  printf("number of tests: %d\ntests passed: %d\n\n", numTests, passCount);
}
