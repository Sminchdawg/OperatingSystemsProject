#include "PartitionFile.h"

struct PartitionFile *partitionOpen(VDIFile f, PartitionEntry partitionEntry) {
    // Combine the open VDI file and the given partition into a single structure and return a pointer to that structure.
    PartitionFile* partitionFile;
    partitionFile -> vdiFile = f;
    partitionFile -> partitionStart = partitionEntry.partitionStart;
    partitionFile -> partitionSize = partitionEntry.partitionSize;

    return partitionFile;
};

void partitionClose(struct PartitionFile *f) {
    // Close the file whose pointer is given
    close(f -> vdiFile.fd);

    // Deallocate any dynamically created memory regions
    delete[] f -> vdiFile.map;
    delete f;
}

ssize_t partitionRead(struct PartitionFile *f,void *buf,size_t count) {
    // Operates the same as vdiRead(). Restrict count so that it does not read beyond the end of the partition.
}

ssize_t partitionWrite(struct PartitionFile *f,void *buf,size_t count) {
    // Operates the same as vdiWrite(). Restrict count so that it does not write beyond the end of the partition.
}

off_t partitionSeek(struct PartitionFile *f,off_t offset,int anchor) {
    // Operates the same as vdiSeek().  Restrict the function so that the cursor remains un-changed if a location outside the partition is requested
    // f -> cursor = f -> vdiFile -> cursor;
    int64_t newCursor;
    newCursor = vdiSeek(f -> vdiFile, f -> vdiFile.header.offData + f -> partitionStart, SEEK_CUR);

    if (newCursor < f -> partitionStart || newCursor > f -> partitionSize + f -> partitionStart) {
        f -> vdiFile.cursor = newCursor;
    }

    return f -> vdiFile.cursor - f -> partitionStart;
}
