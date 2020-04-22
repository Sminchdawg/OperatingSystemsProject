#ifndef INODE_H
#include <iostream>
#include <iomanip>
#include <Ext2File.h>
#include "cmath";
#include <bitset>;
#define INODE_H

struct Inode {
    uint16_t i_mode; // Bytes 1-2
    uint16_t i_uid; // Bytes 3-4
    uint32_t i_size; // Bytes 5-8
    uint32_t i_atime; // Bytes 9-12
    uint32_t i_ctime; // Bytes 13-16
    uint32_t i_mtime; // Bytes 17-20
    uint32_t i_dtime; // Bytes 21-24
    uint16_t i_gid; // Bytes 25-26
    uint16_t i_links_count; // Bytes 27-28
    uint32_t i_blocks; // Bytes 29-32
    uint32_t flags; // Bytes 33-36
    uint32_t i_osd1; // Bytes 37-40
    uint32_t i_block[15]; // Bytes 41-100
    uint32_t i_generation; // Bytes 101-104
    uint32_t i_file_acl; // Bytes 105-108
    uint32_t i_dir_acl; // Bytes 109-112
    uint32_t ifaddr; // Bytes 113-116
    uint32_t i_osd2[3]; // Bytes 117-128
};

int32_t fetchInode(struct Ext2File *f, uint32_t iNum, struct Inode *buf);
int32_t writeInode(struct Ext2File *f, uint32_t iNum, struct Inode *buf);
int32_t inodeInUse(struct Ext2File *f, uint32_t iNum);
int32_t freeInode(struct Ext2File *f, uint32_t iNum);
void displayInode(struct Inode* inode);

#endif // INODE_H
