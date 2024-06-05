#ifndef _LIBTEST_H
#define _LIBTEST_H

#define ASSERT(test, assertion, value) return assertion == value
typedef bool (*TestFunction)();

bool assert(char*, bool, bool);
bool assert_pointer_equals(char*, uint8_t*, uint8_t*);
bool assert_buffer_equals(char*, uint8_t*, uint8_t*, int);
void test(int, TestFunction tests[]);

#endif
