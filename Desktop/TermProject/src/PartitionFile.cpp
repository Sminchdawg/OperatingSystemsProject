#include "PartitionFile.h"
#include <iostream>

using namespace std;

struct PartitionFile *partitionOpen(VDIFile f, PartitionEntry partitionEntry) {
    // Combine the open VDI file and the given partition into a single structure and return a pointer to that structure.
    PartitionFile* partitionFile;
    partitionFile -> vdiFile = f;
    // partitionFile -> partitionStart = partitionEntry.partitionStart;
    // partitionFile -> partitionSize = partitionEntry.partitionSize;

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
    newCursor = vdiSeek(&(f->vdiFile), f -> vdiFile.header.offData + f -> partitionStart, SEEK_CUR);

    if (newCursor < f -> partitionStart || newCursor > f -> partitionSize + f -> partitionStart) {
        f -> vdiFile.cursor = newCursor;
    }

    return f -> vdiFile.cursor - f -> partitionStart;
}

void printPartitionEntry(PartitionEntry partitionEntry) {
    cout << "Status: ";
    if (partitionEntry.status == 00 || partitionEntry.status == 80) {
      cout << "Inactive" << endl;
    } else {
        cout << "Active" << endl;
    }

    cout << "First Sector CHS: ";
        printf("%d", partitionEntry.first_cylinder);
        cout << "-";
        printf("%d", partitionEntry.first_head);
        cout << "-";
        printf("%d", partitionEntry.first_sector);
    cout << endl;

    cout << "First Sector CHS: ";
        printf("%d", partitionEntry.last_cylinder);
        cout << "-";
        printf("%d", partitionEntry.last_head);
        cout << "-";
        printf("%d", partitionEntry.last_sector);
    cout << endl;

    cout << "Partition Type: ";
    if (partitionEntry.type) {
        cout << "83 Linux Native" << endl;
    } else {
        cout << "00 Empty" << endl;
    }

    cout << "First LBA Sector: ";
        printf("%d", partitionEntry.first_lba_sector);
    cout << endl;

    cout << "LBA Sector Count: ";
        printf("%d", partitionEntry.lba_sector_count);
}
