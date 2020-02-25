#ifndef PARTITIONFILE_H
#define PARTITIONFILE_H
#include "VDIFile.h"
#include <iostream>
#include <iomanip>

struct PartitionFile {
    VDIFile* vdiFile;
    int64_t partitionStart;
    int64_t partitionSize;
};

// 16 Bytes
struct PartitionEntry {
    uint8_t status; // Byte 0
    uint8_t first_head; // Byte 1
    uint8_t first_sector; // Byte 2
    uint8_t first_cylinder; // Byte 3
    uint8_t type; // Byte 4
    uint8_t last_head; // Byte 5
    uint8_t last_sector; // Byte 6
    uint8_t last_cylinder; // Byte 7
    uint32_t first_lba_sector; // Bytes 8-11
    uint32_t lba_sector_count; // Bytes 12-15
};

struct PartitionFile *partitionOpen(struct VDIFile *,struct PartitionEntry);
void partitionClose(struct PartitionFile *f);
ssize_t partitionRead(struct PartitionFile *f,void *buf,size_t count);
ssize_t partitionWrite(struct PartitionFile *f,void *buf,size_t count);
off_t partitionSeek(struct PartitionFile *f,off_t offset,int anchor);
void printPartitionEntry(PartitionEntry partitionEntry);

#endif // PARTITIONFILE_H
