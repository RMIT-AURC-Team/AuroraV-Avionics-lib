#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "membuff.h"

bool assert(char* test, bool assertion, bool value) {
  if (test != NULL)
    printf("Running %s\n", test);
  return assertion == value;
}

bool assert_buffer_equals(char* test, uint8_t* buff, uint8_t* goalBuff, int buffSize) {
  if (test != NULL)
    printf("Running %s\n", test);

  printf("Test buffer:\t ");
  for(int i = 0; i < buffSize; i++)
    printf("%d ", buff[i]);
  printf("\n");

  printf("Goal buffer:\t ");
  for(int i = 0; i < buffSize; i++)
    printf("%d ", goalBuff[i]);
  printf("\n");

  return memcmp(buff, goalBuff, buffSize) ? 0 : 1; // memcmp returns 0 if equal
}

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

bool test_flush_empty_buffer() {
  MemBuff mem;
  int buffSize = 12;
  int pageSize = 4;

  uint8_t buff[buffSize];
  MemBuff_init(&mem, buff, buffSize, pageSize);

  // No data appended, buffer should be empty
  uint8_t flushedData[pageSize];
  bool flushReturn = mem.flush(&mem, flushedData);

  // Assert that flushedData is NULL (no data to flush)
  return assert("test_flush_empty_buffer", flushReturn, false);
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
  uint8_t flushedData[pageSize]; 
  bool flushReturn = mem.flush(&mem, flushedData);

  // Assert flushed data matches goal
  return flushReturn && assert_buffer_equals("test_flush_simple_case", flushedData, goalBuff, pageSize);

}

int main() {
  bool (*tests[])() = {
    test_append_no_overflow, test_append_overflow,
    test_flush_empty_buffer, test_flush_simple_case
  }; 

  for(int i = 0; i < sizeof(tests); i++)
    printf("Test %s\n\n", tests[i]() ? "passed" : "failed");

  return 0;
}
