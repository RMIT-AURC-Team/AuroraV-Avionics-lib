#include "Example.h"

void Example_init(Example* ex) {
  ex->integer = 0;
  ex->bar = Example_bar;
}

/********************* INTERNAL INTERFACE *********************/

/* ===============================================================================
 * Foo
 *  - Function that does some foo stuff
 * =============================================================================== */
void _Example_foo(Example* ex) {
}

/********************* PUBLIC INTERFACE *********************/

/* ===============================================================================
 * Bar
 *  - Function that does some bar stuff
 * =============================================================================== */
void Example_bar(Example* ex) {
}
