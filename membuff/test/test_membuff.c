#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "membuff.h"

bool assert_buffer_equals(char* test, uint8_t* buff, uint8_t* goalBuff, int buffSize) {
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
  return assert_buffer_equals("test_append_no_overflow\0", buff, goalBuff, buffSize);
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
  return assert_buffer_equals("test_append_overflow\0", buff, goalBuff, buffSize);
}

int main() {
  bool (*tests[])() = {test_append_no_overflow, test_append_overflow}; 
  for(int i = 0; i < sizeof(tests); i++)
    printf("Test %s\n\n", tests[i]() ? "passed" : "failed");

  return 0;
}
