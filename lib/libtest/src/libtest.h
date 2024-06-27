#ifndef _LIBTEST_H
#define _LIBTEST_H

#include "math.h"

typedef bool (*TestFunction)();

#define ASSERT(test, value, goal)   \
  printf("Asserting "test"\n");     \
  if (value != goal) {              \
    printf("Failed.\n\n");          \
    return false;                   \
  }                                 \
  printf("Success!\n\n");

#define ASSERT_EQ(test, value, goal, datatype)  \
  printf("Asserting "test"\n");                 \
  printf("Test value:\t%"datatype"\n", value);  \
  printf("Goal value:\t%"datatype"\n", goal);   \
  if (value != goal) {                          \
    printf("Failed.\n\n");                      \
    return false;                               \
  }                                             \
  printf("Success!\n\n");

#define ASSERT_EQ_EPS(test, value, goal, epsilon, datatype)  \
  printf("Asserting "test"\n");                              \
  printf("Test value:\t%"datatype"\n", value);               \
  printf("Goal value:\t%"datatype"\n", goal);                \
  if (fabs(value - goal) > epsilon) {                        \
    printf("Epsilon %f\n", value-goal);                      \
    printf("Failed.\n\n");                                   \
    return false;                                            \
  }                                                          \
  printf("Success!\n\n");

#define ASSERT_BUFF_EQ(test, buff, goal, size, datatype)  \
  printf("Asserting "test"\n", test);                     \ 
  printf("Test buffer:\t ");                              \ 
  for(int i = 0; i < size; i++)                           \
    printf("%"datatype" ", buff[i]);                      \
  printf("\n");                                           \
                                                          \
  printf("Goal buffer:\t ");                              \
  for(int i = 0; i < size; i++)                           \
    printf("%"datatype" ", goal[i]);                      \
  printf("\n");                                           \
                                                          \
  if (memcmp(buff, goal, size)) {                         \
    printf("Failed.\n\n");                                \
    return false;                                         \
  }                                                       \
  printf("Success!\n\n");

bool assert(char*, bool, bool);
bool assert_float_equals(char*, float, float);
bool assert_pointer_equals(char*, uint8_t*, uint8_t*);
bool assert_buffer_equals(char*, uint8_t*, uint8_t*, int);
void run_tests(int, TestFunction tests[]);

#endif
