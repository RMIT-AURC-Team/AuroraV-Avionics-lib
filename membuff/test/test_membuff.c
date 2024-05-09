#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "membuff.h"

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

/**************************** APPEND TESTS ****************************/

bool test_append_no_overflow() {
  MemBuff mem;
  int buffSize = 12;
  int pageSize = 4;

  uint8_t buff[buffSize];
  MemBuff_init(&mem, buff, buffSize, pageSize);

  // Write numbers 0-11 to buffer
  for(int i = 0; i < buffSize; i++) {
    mem.append(&mem, i);  // Append integer to buffer
  }
  
  uint8_t goalBuff[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
  return assert_buffer_equals("test_append_no_overflow\0", buff, goalBuff, sizeof(buff));
}

bool test_append_overflow() {
  MemBuff mem;
  int buffSize = 12;
  int pageSize = 4;

  uint8_t buff[buffSize];
  MemBuff_init(&mem, buff, buffSize, pageSize);

  // Write numbers 0-11 to buffer
  for(int i = 0; i < 2*buffSize; i++) {
    mem.append(&mem, i);  // Append integer to buffer
  }
  
  uint8_t goalBuff[] = {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
  return assert_buffer_equals("test_append_overflow\0", buff, goalBuff, sizeof(buff));
}

/**************************** FLUSH TESTS ****************************/

bool test_flush_empty_buffer() {
  MemBuff mem;
  int buffSize = 12;
  int pageSize = 4;

  uint8_t buff[buffSize];
  MemBuff_init(&mem, buff, buffSize, pageSize);

  // No data appended, buffer should be empty
  uint8_t outBuff[pageSize];
  bool flushed = mem.flush(&mem, outBuff);

  // Assert that flush operation returns false
  return assert("test_flush_empty_buffer", flushed, false);
}

bool test_flush_simple_case() {
  MemBuff mem;
  int buffSize = 8;
  int pageSize = 4;

  uint8_t buff[buffSize];
  MemBuff_init(&mem, buff, buffSize, pageSize);

  // Append data to fill the window
  for (int i = 0; i < pageSize; i++) {
    MemBuff_append(&mem, i);
  }

  uint8_t goalBuff[] = {0, 1, 2, 3};
  uint8_t outBuff[pageSize]; 
  bool flushed = mem.flush(&mem, outBuff);

  // Assert flushed data matches goal and window slides forward a page length
  printf("Running test_flush_simple_case\n");
  return flushed && assert_buffer_equals("output buffer", outBuff, goalBuff, pageSize)
                 && assert_pointer_equals("page head", mem.head, buff + pageSize)
                 && assert_pointer_equals("page tail", mem.tail, mem.head + (pageSize - 1));
}

bool test_flush_wrap_around() {
  MemBuff mem;
  int buffSize = 8;
  int pageSize = 4;

  uint8_t buff[buffSize];
  MemBuff_init(&mem, buff, buffSize, pageSize);

  // Append data to overflow the buffer (wrap-around)
  for (int i = 0; i < buffSize + 2; i++) {
    mem.append(&mem, i);
  }

  uint8_t goalBuff[] = {8, 9, 2, 3};
  uint8_t outBuff[buffSize];
  bool flushed = mem.flush(&mem, outBuff);

  // Assert flushed data matches goal (considering wrap-around)
  printf("Running test_flush_wrap_around\n");
  return flushed && assert_buffer_equals("test_flush_wrap_around", outBuff, goalBuff, pageSize)
                 && assert_pointer_equals("page head", mem.head, buff + 4)
                 && assert_pointer_equals("page tail", mem.tail, buff + 7);
}

bool test_flush_partial_window() {
  MemBuff mem;
  int buffSize = 8;
  int pageSize = 4;

  uint8_t buff[buffSize];
  MemBuff_init(&mem, buff, buffSize, pageSize);

  // Append data to partially fill the window
  for (int i = 0; i < 2; i++) {
    mem.append(&mem, i);
  }

  uint8_t goalBuff[] = {0, 1};
  uint8_t outBuff[buffSize];
  bool flushed = mem.flush(&mem, outBuff);

  // Assert flushed data matches partial window and window slides forward by 2 cells
  printf("Running test_flush_partial_window\n");
  return flushed && assert_buffer_equals("test_flush_partial_window", outBuff, goalBuff, 2)
                 && assert_pointer_equals("page head", mem.head, buff + 2)
                 && assert_pointer_equals("page tail", mem.tail, buff + 5);
}

/**********************************************************************/

int main() {
  bool (*tests[])() = {
    test_append_no_overflow, test_append_overflow,
    test_flush_empty_buffer, test_flush_simple_case, 
    test_flush_wrap_around, test_flush_partial_window
  }; 

  for(int i = 0; i < sizeof(tests); i++) {
    printf("\n-----------------------------------------------------------\n");
    printf("Test %s\n", tests[i]() ? "passed" : "failed");
    printf("-----------------------------------------------------------\n");
  }

  return 0;
}
