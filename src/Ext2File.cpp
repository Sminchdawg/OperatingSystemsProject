#include "Ext2File.h"

using namespace std;

struct Ext2File *ext2Open(char *fn, int32_t pNum) {
    struct Ext2File* ext2File = new Ext2File;
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

    // Gets partitionFile
    ext2File -> partitionFile = partitionOpen(f, chosenPartitionEntry);

    // Gets superblock
    struct Superblock* superblock = new Superblock;
    fetchSuperblock(ext2File, 0, superblock);

    // Gets BGD
    struct BGDT* bgdt = new BGDT;
    fetchBGDT(ext2File, 0, bgdt);
    ext2File->bgdt = bgdt;

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
    int spySeek = partitionSeek(f->partitionFile, blockNum * f->file_system_block_size, SEEK_CUR);
    int mistaRead = partitionRead(f->partitionFile, buf, f->file_system_block_size);

    if (spySeek == -1 || mistaRead == -1) {
        cout << "Spy or read has failed in Ext2File for the fetch block" << endl;
        return -1;
    }

    return 0;
}

int32_t writeBlock(struct Ext2File *f, uint32_t blockNum, void *buf) {
    // Write the buffer to the given block in the file system.
    // Return 0 if successful, some other value if the write fails.

    int spySeek = partitionSeek(f->partitionFile, blockNum * f->file_system_block_size, SEEK_CUR);
    int mistaWrite = partitionWrite(f->partitionFile, buf, f->file_system_block_size);

    if (spySeek == -1 || mistaWrite == -1) {
        cout << "Spy or write has failed in Ext2File for the write block" << endl;
        cout << "Spyseek: " << spySeek << endl;
        cout << "Mistawrite: " << mistaWrite << endl;
        return -1;
    }

    return 0;
}

// Superblock functions
int32_t fetchSuperblock(struct Ext2File *f, uint32_t blockNum, struct Superblock *sb) {
    // Read the superblock found in the given block number from the file system into the buffer.

    if (blockNum == 0) {
        int spySeek = partitionSeek(f->partitionFile, 1024, SEEK_CUR);
        int mistaRead = partitionRead(f -> partitionFile, sb, 1024);

        if (spySeek == -1 || mistaRead == -1) {
            cout << "Spy or read has failed in Ext2File for the superblock" << endl;
            return -1;
        }

        f->superBlock = sb;
        f->num_block_groups = ceil((float)f->superBlock->s_blocks_count / (float)f->superBlock->s_blocks_per_group);
        f->bgdt->blockGroups = new Blockgroup[f->num_block_groups];
        if (f->superBlock->s_log_block_size == 0) {
           f->file_system_block_size = 1024;
        } else {
            f->file_system_block_size = 1024 * pow(f->superBlock->s_log_block_size, 2);
        }
    } else {
        fetchBlock(f, blockNum * f->superBlock->s_blocks_per_group + f->superBlock->s_first_data_block, sb);
    }

    // Return 0 for success, non-zero forfailure
    return 0;
}


int32_t writeSuperblock(struct Ext2File *f, uint32_t blockNum, struct Superblock *sb) {
   // Write the superblock to the given block.
   // Return 0 for success, non-zero for failure.

   int spySeek = partitionSeek(f->partitionFile, blockNum * f->file_system_block_size, SEEK_CUR);
   int mistaWrite = partitionWrite(f->partitionFile, sb, 1024);

   if (spySeek == -1 || mistaWrite == -1) {
        cout << "Spy or write has failed in Ext2File for the write block" << endl;
        return -1;
    }

    return 0;
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
    printf("%d\n", superblock->s_first_ino);
    cout << "Inode size: ";
    printf("%d\n", superblock->s_inode_size);
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

int32_t fetchBGDT(struct Ext2File *f, uint32_t blockNum, struct BGDT *bgdt) {
    // Read the block group descriptor table that begins at the given block number.

    bgdt->blockGroups = new Blockgroup[f->num_block_groups];
    if (blockNum == 0) {
        int spySeek;
        if (f->file_system_block_size == 1024) {
                spySeek = partitionSeek(f->partitionFile, f->file_system_block_size * 2, SEEK_CUR);
        } else {
            spySeek = partitionSeek(f->partitionFile, f->file_system_block_size, SEEK_CUR);
        }
        int mistaRead = partitionRead(f->partitionFile, bgdt->blockGroups, 32 * f->num_block_groups);

        if (spySeek == -1 || mistaRead == -1) {
            cout << "Spy or read has failed in Ext2File for the BGDT" << endl;
            return -1;
        }
    } else {

         int spySeek = partitionSeek(f->partitionFile,
         ((blockNum * f->superBlock->s_blocks_per_group) + f->superBlock->s_first_data_block)  * f->file_system_block_size + f->file_system_block_size,
         SEEK_CUR);

        int mistaRead = partitionRead(f->partitionFile, bgdt->blockGroups, 32 * f->num_block_groups);

        if (spySeek == -1 || mistaRead == -1) {
            cout << "Spy or read has failed in Ext2File for the BGDT" << endl;
            return -1;
        }
    }

    // Store the table in the array pointed to by bgdt.
    // Return 0 for success, non-zero for failure.
    return 0;
}

int32_t writeBGDT(struct Ext2File *f, uint32_t blockNum, struct BGDT *bgdt) {
    // Write the block group descriptor table to the file system starting at the given block number.
    // Return 0 for success, non-zero for failure.

   int spySeek = partitionSeek(f->partitionFile, blockNum * f->file_system_block_size, SEEK_CUR);
   int mistaWrite = partitionWrite(f->partitionFile, bgdt->blockGroups, 32 * f->num_block_groups);

   if (spySeek == -1 || mistaWrite == -1) {
        cout << "Spy or write has failed in Ext2File for the write block" << endl;
        cout << "Spyseek: " << spySeek << endl;
        cout << "Mistawrite: " << mistaWrite << endl;
        return -1;
    }

    return 0;
}

void displayBGDT(int num_block_groups, struct BGDT* bgdt) {
    // Display the values in the block group descriptor table.
    cout << "Block group descriptor table: " << endl;
    cout << "Block  " << "Block  " << "Inode  " << "Inode  " << "Free   " << "Free   " << "Used  " << endl;
    cout << "Number " << "Bitmap " << "Bitmap " << "Table  " << "Blocks " << "Inodes " << "Dirs  " << endl;
    cout << "------ " << "------ " << "------ " << "-----  " << "------ " << "------ " << "----  " << endl;
    for (int i = 0; i < num_block_groups; i++) {
        cout << i << "       ";
        printf("%d", bgdt->blockGroups[i].bg_block_bitmap);
        cout << "     ";
        printf("%d", bgdt->blockGroups[i].bg_inode_bitmap);
        cout << "     ";
        printf("%d", bgdt->blockGroups[i].bg_inode_table);
        cout << "     ";
        printf("%d", bgdt->blockGroups[i].bg_free_blocks_count);
        cout << "     ";
        printf("%d", bgdt->blockGroups[i].bg_free_inodes_count);
        cout << "     ";
        printf("%d", bgdt->blockGroups[i].bg_used_dirs_count);
        cout << endl;
    }
}
