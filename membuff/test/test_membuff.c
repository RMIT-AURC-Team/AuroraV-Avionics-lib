#include <stdint.h>
#include <stdio.h>
#include "membuff.h"

int main() {
  MemBuff mem;
  uint8_t buff[12];
  MemBuff_init(&mem, buff, sizeof(buff), 4);

  for(int i = 0; i < 48; i++) {
    if (i%5 == 0) {
      printf("Current head: %p\t", mem.head);
      printf("Current cell: %p\t", mem.cell);
      printf("Current tail: %p\n", mem.tail);

      uint8_t flush[mem.pageSize];
      mem.flush(&mem, flush);
      for(int j = 0; j < sizeof(flush); j++)
          printf("%d ", flush[j]);

      printf("\n");
      printf("New head: %p\t", mem.head);
      printf("New cell: %p\t", mem.cell);
      printf("New tail: %p\n", mem.tail);
      printf("\n\n");
    }

    int x = i%12 ? i : i+10;
    mem.append(&mem, x);
  }

  printf("\n\n");
  for(int i = 0; i < 12; i++)
    printf("%d ", mem.buff[i]);

  return 0;
}
