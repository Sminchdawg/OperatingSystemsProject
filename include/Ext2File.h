#ifndef EXT2FILE_H
#define EXT2FILE_H
#include <iostream>
#include <iomanip>
#include "VDIFile.h"
#include "PartitionFile.h"
#include "cmath";

struct Superblock {
    uint32_t s_inodes_count; // Bytes 0-4
    uint32_t s_blocks_count; // Bytes 5-8
    uint32_t s_r_blocks_count; // Bytes 9-12
    uint32_t s_free_blocks_count; // Bytes 13-16
    uint32_t s_free_inodes_count;  // Bytes 17-20
    uint32_t s_first_data_block; // Bytes 21-24
    uint32_t s_log_block_size; // Bytes 25-28
    uint32_t s_log_frag_size; // Bytes 29-32
    uint32_t s_blocks_per_group; // 33-36
    uint32_t s_frags_per_group; // Bytes 37-40
    uint32_t s_inodes_per_group; // Bytes 41-44
    uint32_t s_mtime; // Bytes 45-48
    uint32_t s_wtime; // Bytes 49-52
    uint16_t s_mnt_count; // Bytes 53-54
    uint16_t s_max_mnt_count; // Bytes 55-56
    uint16_t s_magic; // Bytes 57-58
    uint16_t s_state; // Bytes 59-60
    uint16_t s_errors; // Bytes 61-62
    uint16_t s_minor_rev_lvel; // Bytes 63-64
    uint32_t s_lastcheck; // Bytes 65-68
    uint32_t s_checkinternval; // Bytes 69-72
    uint32_t s_creater_os; // Bytes 73-76
    uint32_t s_rev_level; // Bytes 77-80
    uint16_t s_def_resuid; // Bytes 81-82
    uint16_t s_def_resgid; // Bytes 83-84

    // EXT2_DYNAMIC_REV
    uint32_t s_first_ino; // Bytes 85-88
    uint16_t s_inode_size; // Bytes 89-90s
    uint16_t s_block_group_nr; // Bytes 91-92
    uint32_t s_feature_compat; // Bytes 93-96
    uint32_t s_feature_incompat; // Bytes 97-100
    uint32_t s_feature_ro_compat; // Bytes 101-104
    uint32_t s_uuid[4]; // Bytes 105-120
    uint32_t s_volume_name[4]; // Bytes 121- 136
    uint32_t s_last_mounted[16]; // Bytes 137-200
    uint32_t s_algo_bitmap; // Bytes 201-204

    // Performance Hints
    uint8_t s_prealloc_blocks; // Bytes 205
    uint8_t s_prealloc_dir_blocks; // Bytes 206
    uint16_t alignment; // Bytes 207-208

    // Journaling Support
    uint32_t s_journal_uuid[4]; // Bytes 209-224
    uint32_t s_journal_inum; // Bytes 225-228
    uint32_t s_journal_dev; // Bytes 229-232
    uint32_t s_last_orphan; // Bytes 233-236

    // Directory Indexing Support
    uint32_t s_hash_seed[4]; // Bytes 237-252
    uint8_t s_def_hash_version; // Bytes 253
    uint8_t padding[3]; // Bytes 254-256

    // Other Options
    uint32_t s_default_mount_options; // Bytes 257-260
    uint32_t s_first_meta_bg; // Bytes 261-264
    uint8_t unused[760]; // Bytes 264-1024
};

struct Blockgroup {
    uint32_t bg_block_bitmap; // Bytes 0-4
    uint32_t bg_inode_bitmap; // Bytes 5-8
    uint32_t bg_inode_table; // Bytes 9-12
    uint16_t bg_free_blocks_count; // Bytes 13-14
    uint16_t bg_free_inodes_count; // Bytes 15-16
    uint16_t bg_used_dirs_count; // Bytes 17-18
    uint16_t bg_pad; // Bytes 19-20
    uint8_t bg_reserved[12]; // Bytes 21-32
};

struct BGDT {
    Blockgroup* blockGroups;
};

struct Ext2File {
    PartitionFile* partitionFile;
    Superblock* superBlock = new Superblock;
    BGDT* bgdt = new BGDT;
    uint32_t file_system_block_size;
    int num_block_groups;
};

struct Ext2File *ext2Open(char *fn, int32_t pNum);
void ext2Close(struct Ext2File *f);
int32_t fetchBlock(struct Ext2File * f, uint32_t blockNum, void *buf);
int32_t writeBlock(struct Ext2File *f, uint32_t blockNum, void *buf);
int32_t fetchSuperblock(struct Ext2File *f, uint32_t blockNum, struct Superblock *sb);
int32_t writeSuperblock(struct Ext2File *f, uint32_t blockNum, struct Superblock *sb);
void displaySuperBlock(struct Superblock* superblock);
int32_t fetchBGDT(struct Ext2File *f, uint32_t blockNum, struct BGDT *bgdt);
int32_t writeBGDT(struct Ext2File *f, uint32_t blockNum, struct BGDT *bgdt);
void displayBGDT(int num_block_groups, struct BGDT* bgdt);
int32_t checkValidBlockNum(struct Ext2File* ext2File ,int blockNum);
#endif // EXT2FILE_H
