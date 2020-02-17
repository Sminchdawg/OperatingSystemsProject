#include "PartitionFile.h"
/*
struct PartitionFile *partitionOpen(struct VDIFile *, struct PartitionEntry) {
    // Combine the open VDI file and the given partition into a single structure and return a pointer to that structure.
};

void partitionClose(struct PartitionFile *f) {
    // Close the file whose pointer is given
    close(f -> fd);

    // Deallocate any dynamically created memory regions
    delete[] f -> map;
    delete f;
}

ssize_t partitionRead(struct PartitionFile *f,void *buf,size_t count) {
    // Operates the same asvdiRead(). Restrict count so that it does not read beyond the endof the partition.
}

ssize_t partitionWrite(struct PartitionFile *f,void *buf,size_t count) {
    // Operates the same asvdiWrite(). Restrictcountso that it does not write beyond theend of the partition.
}

off_t partitionSeek(struct PartitionFile *f,off_t offset,int anchor) {
    // Operates the same asvdiSeek().  Restrict the function so that the cursor remains un-changed if a location outside the partition is requested
}
*/
