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
    partitionSeek(ext2File->partitionFile, 1024, SEEK_CUR);
    int read = partitionRead(ext2File->partitionFile, ext2File->superBlock, 1024);

    ext2File->num_block_groups = ext2File->superBlock->s_blocks_count / ext2File->superBlock->s_blocks_per_group;
    ext2File->bgdt->blockGroups = new Blockgroup[ext2File->num_block_groups];


    read = partitionRead(ext2File->partitionFile, ext2File->bgdt, 32);

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

void displaySuperBlock(struct Superblock* superblock) {
    // TODO: Actually format and print out UUID and stuff
    // Display fields labeled and in text form for superblock.
    cout << "Superblock Contents: " << endl;

    cout << "Number of inodes: ";
    printf("%d\n", superblock->s_inodes_count);
    cout << "Number of blocks: ";
    printf("%d\n", superblock->s_blocks_count);
    cout << "Number of reserved blocks: ";
    printf("%d\n", superblock->s_r_blocks_count);
    cout << "Number of free blocks: ";
    printf("%d\n", superblock->s_free_blocks_count);
    cout << "Number of free inodes: ";
    printf("%d\n", superblock->s_free_inodes_count);
    cout << "First data block: ";
    printf("%d\n", superblock->s_first_data_block);
    cout << "Log block size: ";
    printf("%d\n", superblock->s_log_block_size);
    cout << "Log fragment size: ";
    printf("%d\n", superblock->s_log_frag_size);
    cout << "Blocks per group: ";
    printf("%d\n", superblock->s_blocks_per_group);
    cout << "Fragments per group: ";
    printf("%d\n", superblock->s_frags_per_group);
    cout << "Inodes per group: ";
    printf("%d\n", superblock->s_inodes_per_group);
    cout << "Last mount time: ";
    printf("%d\n", superblock->s_mtime);
    cout << "Last write time: ";
    printf("%d\n", superblock->s_wtime);
    cout << "Mount count: ";
    printf("%d\n", superblock->s_mnt_count);
    cout << "Max mount count: ";
    printf("%d\n", superblock->s_max_mnt_count);
    cout << "Magic number: ";
    cout << "0x";
    printf("%02x\n", superblock->s_magic);
    cout << "State: ";
    printf("%d\n", superblock->s_state);
    cout << "Error processing: ";
    printf("%d\n", superblock->s_errors);
    cout << "Revision level: ";
    printf("%d\n", superblock->s_minor_rev_lvel);
    cout << "Last system check: ";
    printf("%d\n", superblock->s_lastcheck);
    cout << "Check interval: ";
    printf("%d\n", superblock->s_checkinternval);
    cout << "OS creator: ";
    printf("%d\n", superblock->s_creater_os);
    cout << "Default reserve UID: ";
    printf("%d\n", superblock->s_rev_level);
    cout << "Default reserve GID: ";
    printf("%d\n", superblock->s_def_resuid);
    cout << "First inode number: ";
    printf("%d\n", superblock->s_def_resgid);
    cout << "Inode size: ";
    printf("%d\n", superblock->s_first_ino);
    cout << "Block group number: ";
    printf("%d\n", superblock->s_block_group_nr);
    cout << "Feature compatibility bits: ";
    cout << "0x";
    printf("%02X\n", superblock->s_feature_compat);
    cout << "Feature incompatibility bits: ";
    cout << "0x";
    printf("%02X\n", superblock->s_feature_incompat);
    cout << "Feature read/only compatibility bits: ";
    cout << "0x";
    printf("%02X\n", superblock->s_feature_ro_compat);
    cout << "UUID: ";
    printf("%d\n", superblock->s_uuid);
    cout << "Volume name: ";
    printf("%d\n", superblock->s_volume_name);
    cout << "Last mount point: ";
    printf("%d\n", superblock->s_last_mounted);
    cout << "Algorithm bitmap: ";
    printf("%d\n", superblock->s_algo_bitmap);
    cout << "Number of blocks to preallocate: ";
    printf("%d\n", superblock->s_prealloc_blocks);
    cout << "Number of blocks to preallocate for directories: ";
    printf("%d\n", superblock->s_prealloc_dir_blocks);
    cout << "Journal UUID: ";
    printf("%d\n", superblock->s_journal_uuid);
    cout << "Journal inode number: ";
    printf("%d\n", superblock->s_journal_inum);
    cout << "Journal device number: ";
    printf("%d\n", superblock->s_journal_dev);
    cout << "Journal last orphan inode number: ";
    printf("%d\n", superblock->s_last_orphan);
    cout << "Default hash version: ";
    printf("%d\n", superblock->s_def_hash_version);
    cout << "Default mount option bitmap: ";
    cout << "0x";
    printf("%02X\n", superblock->s_default_mount_options);
    cout << "First meta block group: ";
    printf("%d\n", superblock->s_first_meta_bg);
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

void displayBGDT(struct BGDT* bgdt) {
    // Display the values in the block group descriptor table.
    /*
    cout << "Block group descriptor table: " << endl;
    cout << "Block  " << "Block  " << "Inode  " << "Inode  " << "Free   " << "Free   " << "Used  " << endl;
    cout << "Number " << "Bitmap " << "Bitmap " << "Table  " << "Blocks " << "Inodes " << "Dirs  " << endl;
    cout << "------ " << "------ " << "------ " << "-----  " << "------ " << "------ " << "----  ";
    for (int i = 0; i < 16; i++) {
        cout << i <<
    }
    */
}
