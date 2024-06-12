#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "libtest.h"
#include "membuff.h"

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
  ASSERT_BUFF_EQ("test_append_no_overflow", buff, goalBuff, sizeof(buff), "d")
  return true;
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
  ASSERT_BUFF_EQ("test_append_overflow", buff, goalBuff, sizeof(buff), "d")
  return true;
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
  ASSERT("test_flush_empty_buffer", flushed, false);
  return true;
}

bool test_flush_simple_case() {
  MemBuff mem;
  int buffSize = 8;
  int pageSize = 4;

  uint8_t buff[buffSize];
  MemBuff_init(&mem, buff, buffSize, pageSize);

  // Append data to fill the window
  for (int i = 0; i < pageSize; i++) {
    mem.append(&mem, i);
  }

  uint8_t goalBuff[] = {0, 1, 2, 3};
  uint8_t outBuff[pageSize]; 
  bool pageReady = mem.readPage(&mem, outBuff);

  // Assert flushed data matches goal and window slides forward a page length
  printf("Running test_flush_simple_case\n");
  printf("Buffer address: %p\n\n", buff);

  ASSERT("page ready", pageReady, true)
  ASSERT_EQ("page head", mem.head, buff + pageSize, "p")
  ASSERT_EQ("page tail", mem.tail, mem.head + (pageSize - 1), "p")
  ASSERT_BUFF_EQ("output buffer", outBuff, goalBuff, pageSize, "d")
  return true;
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

  uint8_t goalBuff1[] = {8, 9, 2, 3};
  uint8_t goalBuff2[] = {0, 0, 0, 0};
  uint8_t outBuff1[buffSize];
  uint8_t outBuff2[buffSize];
  bool pageReady = mem.readPage(&mem, outBuff1);
  bool flushed = mem.flush(&mem, outBuff2);

  // Assert flushed data matches goal (considering wrap-around)
  printf("Running test_flush_wrap_around\n");
  printf("Buffer address: %p\n\n", buff);

  ASSERT("page ready", pageReady, true)
  ASSERT("flush success", flushed, true)
  ASSERT_EQ("page head", mem.head, buff + 2, "p")
  ASSERT_EQ("page tail", mem.tail, buff + 5, "p")
  ASSERT_BUFF_EQ("first output buffer", outBuff1, goalBuff1, pageSize, "d")
  ASSERT_BUFF_EQ("second output buffer", outBuff2, goalBuff2, pageSize, "d")
  return true;
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
  bool pageReady = mem.readPage(&mem, outBuff);
  bool flushed = mem.flush(&mem, outBuff);

  // Assert flushed data matches partial window and window slides forward by 2 cells
  printf("Running test_flush_partial_window\n");
  printf("Buffer address: %p\n\n", buff);

  ASSERT("page not ready", pageReady, false)
  ASSERT("flush success", flushed, true)
  ASSERT_EQ("page head", mem.head, buff + 2, "p")
  ASSERT_EQ("page tail", mem.tail, buff + 5, "p")
  ASSERT_BUFF_EQ("output buffer", outBuff, goalBuff, 2, "d")
  return true;
}

bool test_flush_overflowed_window() {
  MemBuff mem;
  int buffSize = 8;
  int pageSize = 4;

  uint8_t buff[buffSize];
  MemBuff_init(&mem, buff, buffSize, pageSize);

  // Append data to partially fill the window
  for (int i = 0; i < pageSize + 2; i++) {
    mem.append(&mem, i);
  }

  uint8_t goalBuff[] = {0, 1, 2, 3};
  uint8_t outBuff[buffSize];
  bool pageReady = mem.readPage(&mem, outBuff);

  // Assert flushed data matches partial window and window slides forward by 2 cells
  printf("Running test_flush_overflowed_window\n");
  printf("Buffer address: %p\n\n", buff);

  ASSERT("page ready", pageReady, true)
  ASSERT_EQ("page head", mem.head, buff + pageSize, "p")
  ASSERT_EQ("page tail", mem.tail, buff + 2*pageSize - 1, "p")
  ASSERT_BUFF_EQ("output buffer", outBuff, goalBuff, pageSize, "d")
  return true;
}

/**********************************************************************/

int main() {
  TestFunction tests[] = {
    test_append_no_overflow, test_append_overflow,
    test_flush_empty_buffer, test_flush_simple_case, 
    test_flush_wrap_around, test_flush_partial_window, test_flush_overflowed_window
  }; 
  int numTests = sizeof(tests)/sizeof(TestFunction);
  run_tests(numTests, tests);

  return 0;
}
