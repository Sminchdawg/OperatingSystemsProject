#include "Ext2File.h"

using namespace std;

struct Ext2File *ext2Open(char *fn, int32_t pNum) {
    struct Ext2File* ext2File = new Ext2File;
    uint8_t* pageBuffer = new uint8_t[1024];
// Use vdiOpen() and partitionOpen() to open the given VDI file and use partition number pNum.
    VDIFile *f = vdiOpen(fn);

    PartitionEntry partitionEntry[4];
    vdiSeek(f, 446, SEEK_CUR);
    vdiRead(f, partitionEntry, 64);

    PartitionEntry chosenPartitionEntry;

    // Pro tip: Write your code so that pNum = -1 uses the first paritition wiht a Linux file type
    if (pNum == -1) {
        for (int i = 0; i < 4; i++) {
            if (partitionEntry[i].type) {
                chosenPartitionEntry = partitionEntry[i];
            }
        }
    } else {
        chosenPartitionEntry = partitionEntry[pNum];
    }

    // Temp hack
    vdiSeek(f, -446, SEEK_CUR);

    ext2File -> partitionFile = partitionOpen(f, chosenPartitionEntry);
    int read = partitionRead(ext2File -> partitionFile, &(ext2File->superBlock), 1024);
    // printf("%d", &(ext2File->superBlock->s_log_block_size));
    printf("%d", &(ext2File->superBlock->s_inodes_per_group));
    // cout << read;

    // ext2File -> partitionRead(ext2File -> PartitionFile, pageBuffer, 32);
    // Populate all of the fields of your strcture and return a pointer to it.
    return ext2File;
}

void ext2Close(struct Ext2File *f) {
    // close the file whose pointer is given. Deallocate any dynamically created memory regions.
    partitionClose(f -> partitionFile);
}


// Block functions

int32_t fetchBlock(struct Ext2File * f, uint32_t blockNum, void *buf) {
    // Read the given block number from the file system into the buffer.
    // Return 0 if successful, some other value if the write fails.
}

int32_t writeBlock(struct Ext2File *f, uint32_t blockNum, void *buf) {
    // Write the buffer to the given block in the file system.
    // Return 0 if successful, some other value if the write fails.
}


// Superblock functions
/*
int32_t fetchSuperblock(struct Ext2File *f, uint32_t blockNum, struct Ext2Superblock *sb) {
    partitionSeek(f -> partitionFile, 1024, SEEK_CUR);
    partitionRead(f -> partitionFile, buf, 1024);
    // 1024 bytes from the start of the disk patition
    // Read the superblock found in the given block number from the file system into the buffer.
    // Return 0 for success, non-zero for failure
    f -> superblock -> system_block_size = 1024 * Math.pow(2, s_log_block_size);
    f -> superblock -> num_block_groups = s_blocks_count / s_blocks_per_group;
}
*/


int32_t writeSuperblock(struct Ext2File *f, uint32_t blockNum, struct Ext2Superblock *sb) {
   // Write the superblock to the given block.
   // Return 0 for success, non-zero for failure.
}

void displaySuperBlock() {
    // Display fields labeled and in text form for superblock.
}


// Block group descriptor table functions

int32_t fetchBGDT(struct Ext2File *f, uint32_t blockNum, struct Ext2BlockGRoupDescriptor *bgdt) {
    // Read the block group descriptor table that begins at the given block number.
    // Store the table in the array pointed to by bgdt.
    // Return 0 for success, non-zero for failure.
}

int32_t writeBGDT(struct Ext2File *f, uint32_t blockNum, struct Ext2BlockGroupDescriptor *bgdt) {
    // Write the block group descriptor table to the file system starting at the given block number.
    // Return 0 for success, non-zero for failure.
}

void displayBGDT() {
    // Display the values in the block group descriptor table.
}
