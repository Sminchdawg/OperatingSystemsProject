#ifndef PARTITIONFILE_H
#define PARTITIONFILE_H
#include "VDIFile.h"
#include <iostream>
#include <iomanip>

struct PartitionFile {
    VDIFile vdiFile;
    int64_t partitionStart;
    int64_t partitionSize;
};

struct PartitionEntry {
    uint8_t partitionInformation[16];
};

struct PartitionFile *partitionOpen(struct VDIFile *,struct PartitionEntry);
void partitionClose(struct PartitionFile *f);
ssize_t partitionRead(struct PartitionFile *f,void *buf,size_t count);
ssize_t partitionWrite(struct PartitionFile *f,void *buf,size_t count);
off_t partitionSeek(struct PartitionFile *f,off_t offset,int anchor);
void printPartitionEntry(PartitionEntry partitionEntry);

#endif // PARTITIONFILE_H
