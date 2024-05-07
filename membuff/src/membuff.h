#ifndef _MEMBUFF_H
#define _MEMBUFF_H

typedef struct MemBuff {
  uint8_t* buff;
  uint8_t* buffEnd; // Last cell in buffer
  uint8_t* head;    // Page window head
  uint8_t* tail;    // Page window tail
  uint8_t* cell;    // Current cell in buffer
  int pageSize; 
  int buffSize;
  int length; 
  void (*_slide)(struct MemBuff*, uint8_t*);
  void (*flush)(struct MemBuff*, uint8_t*);
  void (*append)(struct MemBuff*, uint8_t);
  void (*erase)(struct MemBuff*, uint8_t*, uint8_t*);
} MemBuff;

void _MemBuff_slide(MemBuff*, uint8_t*);

void MemBuff_init(MemBuff*, uint8_t*, int, int);
void MemBuff_flush(MemBuff*, uint8_t*);
void MemBuff_append(MemBuff*, uint8_t);
void MemBuff_erase(struct MemBuff*, uint8_t*, uint8_t*);

#endif
