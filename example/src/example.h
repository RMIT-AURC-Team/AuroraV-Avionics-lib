#ifndef _EXAMPLE_H
#define _EXAMPLE_H

typedef struct Example {
  int integer;
  void (*bar)(struct Example*);
} Example;

void _Example_foo(Example*);
void Example_bar(Example*);

#endif
