#ifndef DisplayBuffer.h
#include <cstdint>
#define DisplayBuffer.h

void displayBufferPage(void *buf,uint32_t count,uint32_t start,uint64_t offset);
void displayBuffer(void *buf, uint32_t count, uint64_t offset);

#endif // DISPLAYBUFFER_H
