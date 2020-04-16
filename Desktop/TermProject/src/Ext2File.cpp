#include "Ext2File.h"

struct Ext2File *ext2Open(char *fn, int32_t pNum) {
    // Use vdiOpen() and partitionOpen() to open the given VDI file and use partition number pNum.
    // Populate all of the fields of your strcture and return a pointer to it.
    // Pro tip: Write your code so that pNum = -1 uses the first paritition wiht a Linux file type
}

void ext2Close(struct Ext2File *f) {
    // close the file whose pointer is given. Deallocate any dynamically created memory regions.
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

int32_t fetchSuperblock(struct Ext2File *f, uint32_t blockNum, struct Ext2Superblock *sb) {
    partitionSeek(f -> partitionFile, 1024, SEEK_CUR);
    partitionRead(f -> partitionFile, buf, 1024);
    // 1024 bytes from the start of the disk patition
    // Read the superblock found in the given block number from the file system into the buffer.
    // Return 0 for success, non-zero for failure
    f -> superblock -> system_block_size = 1024 * Math.pow(2, s_log_block_size);
    f -> superblock -> num_block_groups = s_blocks_count / s_blocks_per_group;
}

int32_t writeSuperblock(struct Ext2File *f, uint32_t blockNum, struct Ext2Superblock *sb) {
   // WRite the superblock to the given block.
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
