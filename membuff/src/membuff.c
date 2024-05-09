#include <stdlib.h>
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
  mem->pageReady = false;

  mem->readPage = MemBuff_readPage;
  mem->flush = MemBuff_flush;
  mem->append = MemBuff_append;
  mem->erase = MemBuff_erase;
}

/******************** INTERNAL INTERFACE ********************/

void _MemBuff_slide(MemBuff* mem, uint8_t* newHead) {
  int pageEnd = mem->pageSize - 1;
  bool headOverflow = (newHead >= mem->buffEnd);
  bool tailOverflow = (newHead + pageEnd) >= mem->buffEnd;

  if (headOverflow) {
    // If head extends beyond the buffer
    int diff = (int)(newHead - mem->buffEnd);
    mem->head = mem->buff + diff;           // New head loops back difference from start of buffer
    mem->tail = mem->head + pageEnd;        // New tail is simply a page length increment from new head
  } else if (tailOverflow) {
    // If tail extends beyond the buffer
    int diff = (int)((newHead + pageEnd) - mem->buffEnd);
    mem->head = newHead;                    // New head is simply the passed pointer
    mem->tail = mem->buff + diff;           // New tail loops back difference from start of buffer
  } else {
    // Base case
    mem->head = newHead;
    mem->tail = newHead + pageEnd;
  }
}

/********************* PUBLIC INTERFACE *********************/

/* ===============================================================================
 * APPEND
 * - Adds data to the next cell in the internal buffer, handles overflow.
 * - Increments length after append and sets flag if page window is full;
 * =============================================================================== */
void MemBuff_append(MemBuff* mem, uint8_t data) {
  // Circle cell pointer back to start of buffer if past end
  if (mem->cell == mem->buffEnd) {
    mem->cell = mem->buff;
  }

  *mem->cell = data;
  mem->cell++;

  // Increment length and set ready flag if page window is filled
  ++mem->length;
  if (mem->length >= mem->pageSize) {
    mem->pageReady = true;          
  }
}

/* ===============================================================================
 * READPAGE
 * - Flushes data to output buffer if page window is ready to be read
 *    and resets flag.
 * =============================================================================== */
bool MemBuff_readPage(MemBuff* mem, uint8_t* outBuff) {
  // Early exit with false if no page ready
  if (!mem->pageReady) {
    return false;
  }

  // Otherwise reset flag and pass out buffer
  mem->pageReady = false;
  mem->flush(mem, outBuff);  // Flush to output buffer
  return true;
}

/* ===============================================================================
 * FLUSH
 * - Copies out data currently within page window to output buffer and 
 *    slides page window forward.
 * - Used by readPage function to handle guarded output, can be used standalone
 *    for early buffer reads.
 *
 * Caution should be taken when calling directly, problems can occur if
 *    the internal buffer overflows back to the current window. It is recommended 
 *    to use readPage where possible to flush out the window when full.
 * =============================================================================== */
bool MemBuff_flush(MemBuff* mem, uint8_t* outBuff) {
  // Early exit with false if nothing to flush
  if (mem->length == 0) {
    return false;
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
  return true;
}

/* ===============================================================================
 * ERASE
 * - Set cells to zero between start and end pointers.
 * - Used by the flush function to prevent writing out old data to memory.
 * =============================================================================== */
void MemBuff_erase(MemBuff* mem, uint8_t* start, uint8_t* end) {
  int diff = (int)(end - start);
  uint8_t* cell = start;
  for(int i = 0; i <= diff; i++) {
    // Loop back to start of buffer on overflow
    if (cell >= mem->buffEnd)
        cell = mem->buff;
    *cell = 0;
    cell++;
  }
}
