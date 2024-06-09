#ifndef _LIBTEST_H
#define _LIBTEST_H

typedef bool (*TestFunction)();

#define ASSERT_EQ(test, value, goal)  \
  printf("Asserting %s\n", test);     \
  printf("Test value:\t%f\n", value); \
  printf("Goal value:\t%f\n", goal);  \
  if (value != goal) {                \
    printf("Failed.\n\n");            \
    return false;                     \
  }                                   \
  printf("Success!\n\n");

bool assert(char*, bool, bool);
bool assert_float_equals(char*, float, float);
bool assert_pointer_equals(char*, uint8_t*, uint8_t*);
bool assert_buffer_equals(char*, uint8_t*, uint8_t*, int);
void run_tests(int, TestFunction tests[]);

#endif
