#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "membuff.h"

void MemBuff_init(MemBuff* mem, uint8_t* buff, int buffSize, int pageSize) {
  mem->buff = buff;
  mem->buffSize = buffSize;
  mem->pageSize = pageSize;
  mem->length = 0;
  mem->head = mem->buff;                    // Initialise head at buff[0] 
  mem->cell = mem->head;                    // Initialise cell pointer at head
  mem->tail = mem->head + mem->pageSize-1;  // Initialise tail incremented a page length from head
  mem->buffEnd = mem->buff + mem->buffSize;

  mem->_slide = _MemBuff_slide;

  mem->flush = MemBuff_flush;
  mem->append = MemBuff_append;
  mem->erase = MemBuff_erase;
}

/******************** INTERNAL INTERFACE ********************/
void _MemBuff_slide(MemBuff* mem, uint8_t* newHead) {
  bool headOverflow = (newHead >= mem->buffEnd);
  bool tailOverflow = (newHead + mem->pageSize) >= mem->buffEnd;

  if (headOverflow) {
    // If head extends beyond the buffer
    int diff = (int)(newHead - mem->buffEnd);
    mem->head = mem->buff + diff;           // New head loops back difference from start of buffer
    mem->tail = mem->head + mem->pageSize;  // New tail is simply a page length increment from new head
  } else if (tailOverflow) {
    // If tail extends beyond the buffer
    int diff = (int)((newHead + mem->pageSize) - mem->buffEnd);
    mem->head = newHead;                    // New head is simply the passed pointer
    mem->tail = mem->buff + diff;           // New tail loops back difference from start of buffer
  } else {
    // Base case
    mem->head = newHead;
    mem->tail = newHead + mem->pageSize;
  }
}

/********************* PUBLIC INTERFACE *********************/
void MemBuff_append(MemBuff* mem, uint8_t data) {
  // Circle cell pointer back to start of buffer if past end
  if (mem->cell == mem->buffEnd) {
    mem->cell = mem->buff;
  }

  *mem->cell = data;
  mem->cell++;
  mem->length++;
}

void MemBuff_flush(MemBuff* mem, uint8_t* outBuff) {
  // Output null end return if nothing to flush
  if (mem->length == 0) {
    outBuff = NULL;
    return;
  }

  // Copy data from page window to output buffer
  if (mem->head < mem->tail) {
    // If page window is within the buffer 
    memcpy(outBuff, mem->head, mem->pageSize);   // Copy in data from head to tail
  } else {
    // If page window overflows buffer
    int diff = (int)(mem->buffEnd - mem->head);
    int startDiff = mem->pageSize - diff;
    memcpy(outBuff, mem->head, diff);             // Copy in data from head to end of buffer
    memcpy(outBuff + diff, mem->buff, startDiff); // Copy in data from start of buffer to tail
  }

  // Slide head to next page window
  if (mem->length >= mem->pageSize) {
    // If cell has overflowed page window
    mem->_slide(mem, (mem->head + mem->pageSize)); // Push head and tail forward a page length
    mem->length = (mem->cell - mem->head);         // Calculate new length from cell overflow
  } else {
    // If cell is still within page window
    mem->_slide(mem, mem->cell);                   // Push head and tail forward to start at current cell
    mem->length = 0;                               // Reset length to 0
  }

  // Erase page window from current cell onward
  mem->erase(mem, mem->cell, mem->tail);
}

void MemBuff_erase(MemBuff* mem, uint8_t* start, uint8_t* end) {
  int diff = (int)(end - start);
  uint8_t* cell = start;
  for(int i = 0; i < diff; i++) {
    // Loop back to start of buffer on overflow
    if (cell >= mem->buffEnd)
        cell = mem->buff;
    *cell = 0;
    cell++;
  }
}
