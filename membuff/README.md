## MemBuff Circular Memory Buffer

This library provides a lightweight implementation of a circular buffer in C, designed to be implemented for writing to flash memory. 

### Usage

**1. Include header:**

```C
#include "membuff.h"
```
**2. Initialize buffer:**

```C
uint8_t buffer_data[16]; // Buffer data storage
int buffer_size = sizeof(buffer_data);
int page_size = 4;

MemBuff mem;
mem.init(&mem, buffer_data, buffer_size, page_size);

```

**3. Add data:**

Use `MemBuff_append` to add data to the buffer. It automatically handles wrapping around when the buffer fills up.

```C
for (int i = 0; i < 10; i++) {
  mem.append(&mem, i);
}
```

**4. Flush buffer:**

Use `MemBuff_flush` to discard data in the current page window and move the window forward. This is useful when you want to start writing fresh data (i.e. after writing to flash).

```C
mem.flush(&mem);
```

**5. Erase data:**

Use `MemBuff_erase` to manually erase a specific range of data within the buffer.

```C
mem.erase(&mem, buffer_data + 2, buffer_data + 5); // Erase data from index 2 to 4
```

**Additional features:**

-   `mem.length`: Provides the current length of data in the buffer.
