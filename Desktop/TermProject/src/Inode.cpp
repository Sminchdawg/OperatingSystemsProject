#include "Inode.h"

using namespace std;

int32_t fetchInode(struct Ext2File *f, uint32_t iNum, struct Inode *buf) {
    // Read the inode whose index number is iNum and store the information in the buffer
    int iNodesPerBlock = f->file_system_block_size / f->superBlock->s_inode_size;
    int blockGroup = floor(((float)iNum - 1) / ((float)f->superBlock->s_inodes_per_group));
    cout << "block group: " << blockGroup << endl;
    int iNumIndex = (iNum -1) % f->superBlock->s_inodes_per_group;
    cout << "inum index: " <<  iNumIndex << endl;
    int blockNum = floor((float)iNumIndex / (float)iNodesPerBlock) + f->bgdt->blockGroups[blockGroup].bg_inode_table;
    cout << "block num: " << blockNum  << endl;
    iNumIndex = iNumIndex % iNodesPerBlock;
    cout << "inum index: " <<  iNumIndex << endl;


    Inode* iNodes;
    iNodes = new Inode[iNodesPerBlock];
    fetchBlock(f, blockNum, iNodes);
    *buf = iNodes[iNumIndex];

    return 0;
}

int32_t writeInode(struct Ext2File *f, uint32_t iNum, struct Inode *buf) {
    // Write the inode whose index number is Inum from the given buffer
    int iNodesPerBlock = f->file_system_block_size / f->superBlock->s_inode_size;
    int blockGroup = floor(((float)iNum - 1) / ((float)f->superBlock->s_inodes_per_group));
    int iNumIndex = (iNum -1) % f->superBlock->s_inodes_per_group;
    int blockNum = floor((float)iNumIndex / (float)iNodesPerBlock) + f->bgdt->blockGroups[blockGroup].bg_inode_table;
    iNumIndex = iNumIndex % iNodesPerBlock;

    Inode* iNodes;
    iNodes = new Inode[iNodesPerBlock];
    fetchBlock(f, blockNum, iNodes);
    iNodes[iNumIndex] = *buf;
    writeBlock(f, blockNum, iNodes);
}

int32_t inodeInUse(struct Ext2File *f, uint32_t iNum) {
    // Returns turn if the inode is marked as allocated and not false if its not marked
    uint8_t* bitMap = new uint8_t[1024];
    int iNodesPerBlock = f->file_system_block_size / f->superBlock->s_inode_size;
    int blockGroup = floor(((float)iNum - 1) / ((float)f->superBlock->s_inodes_per_group));
    int iNumIndex = (iNum -1) % f->superBlock->s_inodes_per_group;
    int blockNum = floor((float)iNumIndex / (float)iNodesPerBlock) + f->bgdt->blockGroups[blockGroup].bg_inode_table;
    fetchBlock(f, f->bgdt->blockGroups[blockGroup].bg_inode_bitmap, bitMap);

    cout << (1 << bitMap[iNum]) << endl;
    if (bitMap[iNum] == 0) {
        return false;
    } else {
        return true;
    }
}

uint32_t allocateInode(struct Ext2File *f, int32_t group) {
    // Select any unmarked inode in the given block group, mark it as allocated and return its inode number
    // If the group is -1, the select any available inode fromany group
    uint32_t iNum;
    bool notfound;
    int loop = 0;
    uint8_t* bitMap = new uint8_t[1024];

    if (group == -1) {
        while(notFound && loop < f->num_block_groups) {
            fetchBlock(f, f->bgdt->blockGroups[group], bitMap);
            // Loop through entries in bitMap and find first one that has 0 value.
            // Set to allocated, and return the index
            loop++;
        }
    } else {
        fetchBlock(f, f->bgdt->blockGroups[group], bitMap);
        // Loop through entries in bitMap and find first one that has 0 value.
        // Set to allocated, and return the index
    }

    return iNum;
}

int32_t freeInode(struct Ext2File *f, uint32_t iNum) {
    // Marks the given inode as free
    uint8_t* bitMap = new uint8_t[1024];
    int iNodesPerBlock = f->file_system_block_size / f->superBlock->s_inode_size;
    int blockGroup = floor(((float)iNum - 1) / ((float)f->superBlock->s_inodes_per_group));
    int iNumIndex = (iNum -1) % f->superBlock->s_inodes_per_group;
    int blockNum = floor((float)iNumIndex / (float)iNodesPerBlock) + f->bgdt->blockGroups[blockGroup].bg_inode_table;

    fetchBlock(f, blockNum, bitMap);
    cout << "bitmap inum: ";
    printf("%d\n", bitMap[iNum]);
    bitMap[iNum] = 0;
    cout << "bitmap inum: " << bitMap[iNum] << endl;
    writeBlock(f, blockNum, bitMap);

    return 0;
}

void displayInode(struct Inode* iNode) {
    cout << "Mode: ";
    printf("%o\n", iNode->i_mode);
    cout << "Size: ";
    printf("%d\n", iNode->i_size);
    cout << "Blocks: ";
    printf("%d\n", iNode->i_blocks);
    cout << "UID / GID: ";
    printf("%d", iNode->i_uid);
    cout << " / ";
    printf("%d\n", iNode->i_gid);
    cout << "Links: ";
    printf("%d\n", iNode->i_links_count);
    cout << "Created: ";
    printf("%d\n", iNode->i_ctime);
    cout << "Last access: ";
    printf("%d\n", iNode->i_atime);
    cout << "Last modification: ";
    printf("%d\n", iNode->i_mtime);
    cout << "Deleted: ";
    printf("%d\n", iNode->i_dtime);
    cout << "Flags: ";
    printf("%d\n", iNode->flags);
    cout << "File version: ";
    printf("%d\n", iNode->i_file_acl);
    cout << "ACL block: ";
    printf("%d\n", iNode->ifaddr);
    cout << "Direct blocks: ";
    cout << "0-3: ";
    printf("%d\n", iNode->ifaddr);
    cout << "4-7: ";
    printf("%d\n", iNode->ifaddr);
    cout << "8-11: ";
    printf("%d\n", iNode->ifaddr);
    cout << "Single indirect block: ";
    printf("%d\n", iNode->ifaddr);
    cout << "Double indirect block: ";
    printf("%d\n", iNode->ifaddr);
    cout << "Triple indirect block: ";
    printf("%d\n", iNode->ifaddr);
}
