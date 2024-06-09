#include "libtest.h"
#include "example.h"

/**************************** TESTS ****************************/

bool test_foo() {
}

/***************************************************************/

int main() {
  TestFunction tests[] = {
    test_foo
  }; 
  int numTests = sizeof(tests)/sizeof(TestFunction);
  run_tests(numTests, tests);

  return 0;
}
