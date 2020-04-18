#include "Inode.h"

using namespace std;

int32_t fetchInode(struct Ext2File *f, uint32_t iNum, struct Inode *buf) {
    // Read the inode whose index number is iNum and store the information in the buffer
    int iNodesPerGroup = f->file_system_block_size / f->superBlock->s_inode_size;
    int blockGroup = floor(((float)iNum - 1) / ((float)f->superBlock->s_inodes_per_group));
    int iNumIndex = iNum - (blockGroup * f->superBlock->s_inodes_per_group);
    // int blockNum = f->bgdt->blockGroups[blockGroup].bg_inode_table + floor((float)(iNumIndex * f->superBlock->s_inode_size) / (float)f->file_system_block_size);
    int blockNum = f->bgdt->blockGroups[blockGroup].bg_inode_table + floor((float)(iNumIndex * f->superBlock->s_inode_size) / (float)f->file_system_block_size);
    cout << "block num: " << blockNum << endl;
    cout << "block group: " << blockGroup << endl;
    Inode* iNodes;
    iNodes = new Inode[iNodesPerGroup];
    fetchBlock(f, blockNum + 1, iNodes);
    cout << iNumIndex % iNodesPerGroup << endl;
    *buf = iNodes[iNumIndex % iNodesPerGroup];

}

int32_t writeInode(struct Ext2File *f, uint32_t iNum, struct Inode *buf) {
    // Write the inode whose index number is Inum from the given buffer
}

int32_t inodeInUse(struct Ext2File *f, uint32_t iNum) {
    // Returns 0 if the inode is marked as allocated and not-zero if its not marked
}

int32_t freeInode(struct Ext2File *f, uint32_t iNum) {
    // Marks the given inode as free
}

void displayInode(struct Inode* iNode) {
    cout << "Mode: ";
    printf("%d\n", iNode->i_mode);
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
