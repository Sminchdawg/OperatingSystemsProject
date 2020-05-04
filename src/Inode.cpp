#include "Inode.h"

using namespace std;

int32_t fetchInode(struct Ext2File *f, uint32_t iNum, struct Inode *buf) {
    // Read the inode whose index number is iNum and store the information in the buffer

    int iNodesPerBlock = f->file_system_block_size / f->superBlock->s_inode_size;
    int blockGroup = floor(((float)iNum - 1) / ((float)f->superBlock->s_inodes_per_group));
    int iNumIndex = (iNum -1) % f->superBlock->s_inodes_per_group;
    int blockNum = floor((float)iNumIndex / (float)iNodesPerBlock) + f->bgdt->blockGroups[blockGroup].bg_inode_table;
    // cout << blockNum << endl;
    // cout << f->bgdt->blockGroups[blockGroup].bg_inode_table;
    iNumIndex = iNumIndex % iNodesPerBlock;

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
    int iNumIndexInBlock = (iNum -1) % f->superBlock->s_inodes_per_group;
    int blockNum = f->bgdt->blockGroups[blockGroup].bg_inode_bitmap;
    int iNumIndexInByte = iNumIndexInBlock % 8;
    fetchBlock(f, blockNum, bitMap);
    cout << "block num in use: " << blockNum << endl;
    cout << "inum in block: " << iNumIndexInBlock << endl;
    cout << "inum in byte: " << iNumIndexInByte << endl;

    if (bitMap[iNumIndexInBlock] & (1 << (iNumIndexInByte - 1))) {
        return true;
    } else {
         return false;
    }
}

uint32_t allocateInode(struct Ext2File *f, int32_t group) {
    // Select any unmarked inode in the given block group, mark it as allocated and return its inode number
    // If the group is -1, the select any available inode fromany group
    uint32_t iNum;
    uint8_t* bitMap = new uint8_t[f->file_system_block_size];
    int start = 0;
    int end = f->num_block_groups;

    if (group > -1) {
        start = group;
        end = group + 1;
    }

   for (int i = start; i < end; i++) {
        fetchBlock(f, f->bgdt->blockGroups[i].bg_inode_bitmap, bitMap);
        for (int j = 0; j < f->file_system_block_size; j++) {
            if (bitMap[j] != 0xff) {
                cout << "This is J: " << j << endl;
                for (int k = 0; k < 8; k++) {
                    if (bitMap[j] & (1 << (k))) {
                        cout << "in use" << endl;
                    } else {
                        bitMap[j] |= 1 << k;

                        iNum = (k + 8 * j) + 1 + (i * f->superBlock->s_inodes_per_group);
                        writeBlock(f, f->bgdt->blockGroups[i].bg_inode_bitmap, bitMap);
                        f->superBlock->s_free_inodes_count--;
                        writeSuperblock(f, 0, f->superBlock);
                        f->bgdt->blockGroups[i].bg_free_inodes_count--;
                        writeBGDT(f, 0, f->bgdt);
                        return iNum;
                    }
                }
            }
        }
   }

    return 0;
}

int32_t freeInode(struct Ext2File *f, uint32_t iNum) {
    // Marks the given inode as free
    uint8_t* bitMap = new uint8_t[f->file_system_block_size];
    int iNodesPerBlock = f->file_system_block_size / f->superBlock->s_inode_size;
    int blockGroup = floor(((float)iNum - 1) / ((float)f->superBlock->s_inodes_per_group));
    int iNumIndexInBlock = (iNum-1) % f->superBlock->s_inodes_per_group;
    int blockNum = f->bgdt->blockGroups[blockGroup].bg_inode_bitmap;
    int iNumIndexInByte = iNumIndexInBlock % 8;

    fetchBlock(f, blockNum, bitMap);

    bitMap[iNumIndexInBlock] = bitMap[iNumIndexInBlock] & (1 << ~(iNumIndexInByte - 1));

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
    cout << "Direct blocks: " << endl;
    cout << "0-3: ";
    printf("%d", iNode->i_block[0]);
    cout << " ";
    printf("%d", iNode->i_block[1]);
    cout << " ";
    printf("%d", iNode->i_block[2]);
    cout << " ";
    printf("%d\n", iNode->i_block[3]);
    cout << "4-7: ";
    printf("%d", iNode->i_block[4]);
    cout << " ";
    printf("%d", iNode->i_block[5]);
    cout << " ";
    printf("%d", iNode->i_block[6]);
    cout << " ";
    printf("%d\n", iNode->i_block[7]);
    cout << "8-11: ";
    printf("%d", iNode->i_block[8]);
    cout << " ";
    printf("%d", iNode->i_block[8]);
    cout << " ";
    printf("%d", iNode->i_block[10]);
    cout << " ";
    printf("%d\n", iNode->i_block[11]);
    cout << "Single indirect block: ";
    printf("%d\n", iNode->i_block[12]);
    cout << "Double indirect block: ";
    printf("%d\n", iNode->i_block[13]);
    cout << "Triple indirect block: ";
    printf("%d\n", iNode->i_block[14]);
}
