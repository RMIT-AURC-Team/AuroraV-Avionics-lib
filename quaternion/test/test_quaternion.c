#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "libtest.h"
#include "quaternion.h"

/**********************************************************************/

// Tolerance for floating point comparison
bool test_quaternion_normalise() {
    printf("Running test_quaternion_normalise\n");

    Quaternion q;
    Quaternion_init(&q);
    q.w = 1.0f;
    q.x = 2.0f;
    q.y = 3.0f;
    q.z = 4.0f;
    q.normalise(&q);

    float magnitude = sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);

    ASSERT_EQ_EPS("w", q.w, 0.18f, 0.01f, "f");
    ASSERT_EQ_EPS("x", q.x, 0.37f, 0.01f, "f");
    ASSERT_EQ_EPS("y", q.y, 0.55f, 0.01f, "f");
    ASSERT_EQ_EPS("z", q.z, 0.73f, 0.01f, "f");
    ASSERT_EQ_EPS("Magnitude", magnitude, 1.0f, 0.01f, "f");
    return true;
}

bool test_quaternion_multiply() {
    Quaternion q1;
    Quaternion_init(&q1);
    q1.w = 1.0f;
    q1.x = 0.0f;
    q1.y = 0.0f;
    q1.z = 0.0f;

    Quaternion q2;
    Quaternion_init(&q2);
    q2.w = 0.0f;
    q2.x = 1.0f;
    q2.y = 0.0f;
    q2.z = 0.0f;

    Quaternion result = Quaternion_mul(&q1, &q2);

    ASSERT_EQ("w", result.w, 0.0f, "f");
    ASSERT_EQ("x", result.x, 1.0f, "f");
    ASSERT_EQ("y", result.y, 0.0f, "f");
    ASSERT_EQ("z", result.z, 0.0f, "f");
    return true;
}

int main() {
  TestFunction tests[] = {
        test_quaternion_normalise, test_quaternion_multiply
    }; 
  int numTests = sizeof(tests)/sizeof(TestFunction);
  run_tests(numTests, tests);

  return 0;
}
