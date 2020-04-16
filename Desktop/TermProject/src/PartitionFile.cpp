#include "PartitionFile.h"
#include "VDIFile.h"
#include <iostream>

using namespace std;

struct PartitionFile *partitionOpen(struct VDIFile *f, struct PartitionEntry partitionEntry) {
    // Combine the open VDI file and the given partition into a single structure and return a pointer to that structure.
    struct PartitionFile* partitionFile = new PartitionFile;
    partitionFile -> vdiFile = f;

    partitionFile -> partitionStart = partitionEntry.first_lba_sector * 512;
    partitionFile -> partitionSize = partitionEntry.lba_sector_count * 512;

    return partitionFile;
};

void partitionClose(struct PartitionFile *f) {
    vdiClose(f -> vdiFile);
}

ssize_t partitionRead(struct PartitionFile *f,void *buf,size_t count) {
    // Operates the same as vdiRead(). Restrict count so that it does not read beyond the end of the partition.
    int bytesRead;

    if (count > f -> partitionSize) {
        cout << "Too big count paps";
    } else {
        bytesRead = vdiRead(f -> vdiFile, buf, count);
    }
    return bytesRead;
}

ssize_t partitionWrite(struct PartitionFile *f,void *buf,size_t count) {
    // Operates the same as vdiWrite(). Restrict count so that it does not write beyond the end of the partition.
    int bytesWrote;

    if (count > f -> partitionSize) {
        cout << "Too big count paps";
    } else {
        bytesWrote = vdiWrite(f -> vdiFile, buf, count);
    }
    return bytesWrote;
}

off_t partitionSeek(struct PartitionFile *f,off_t offset,int anchor) {
    // Operates the same as vdiSeek().  Restrict the function so that the cursor remains un-changed if a location outside the partition is requested
    // f -> cursor = f -> vdiFile -> cursor;
    uint64_t newCursor = 0;

    if (!newCursor < f -> partitionStart || !newCursor > f -> partitionSize + f -> partitionStart) {
        newCursor = vdiSeek(f-> vdiFile, f -> partitionStart + offset, SEEK_CUR);
    } else {
        cout << "The requested location is outside of the boundaries of that partition";
    }
    return newCursor;
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
