#include "FileAccess.h"
#include "Inode.h"

using namespace std;

int32_t fetchBlockFromFile(struct Ext2File *f, struct Inode *i, uint32_t bNum, void *buf) {
    int k = f->file_system_block_size / 4;

    uint32_t* blockList = new uint32_t[f->file_system_block_size / 4];
    int index;
    if (bNum < 12) {
        blockList = i->i_block;

        goto direct;
    } else if (bNum < (12 + k)) {
        // ^^^ Index is in the first single indirect block, aka i_block[12]
        if (i->i_block == 0) {
            return false;
        } else {
            fetchBlock(f, i->i_block[12], blockList);
            bNum -= 12;

            goto direct;
        }
    } else if (bNum < 12 + k + k * k) {
        cout << "The block is in the first double indirect block" << endl;
        // ^^ Index is in first double indirect block, aka i_block[13]
        if (i->i_block[13] == 0) {
            return false;
        } else {
            fetchBlock(f, i->i_block[13], blockList);
            bNum -= (12 + k);
            cout << "bNum after sub: " << bNum << endl;

            goto single;
        }
    } else {
        cout << "The block is in the first triple indirect block" << endl;
        // ^^ Index is in the first triple indirect block, aka i_block[14]
        if (i->i_block[14] == 0) {
            return false;
        } else {
            fetchBlock(f, i->i_block[14], blockList);

            long kPow = k * k;

            bNum -= (12 + k + kPow);

            index = bNum / (k * k);
            bNum = bNum % kPow;

            if (blockList[index] == 0) {
                return false;
            } else {
                fetchBlock(f, blockList[index], blockList);
            }
        }
    }


    // Single
    single:
    cout << "bNum at single: " << bNum << endl;
    index = bNum / k;
    bNum = bNum % k;
    cout << "index: " << index << endl;
    cout << "bNum: " << bNum << endl;

    if (blockList[index] == 0) {
        return false;
    } else {
        fetchBlock(f, blockList[index], blockList);
    }

    // Direct
    direct:
    if (blockList[bNum] == 0) {
        return false;
    } else {
        fetchBlock(f, blockList[bNum], buf);
        return true;
    }
}

int32_t writeBlockToFile(struct Ext2File *f, struct Inode *i, uint32_t bNum, void *buf, uint32_t iNum) {
    // Write the given buffer into the block bNum in the file
    // this may involve allocating one or more data blocks
    int k = f->file_system_block_size / 4;

    uint32_t* blockList = new uint32_t[f->file_system_block_size / 4];
    uint32_t* temp = new uint32_t[f->file_system_block_size / 4];
    uint32_t ibNum;
    int index;

    if (bNum < 12) {
        if (i->i_block[bNum] == 0) {
            i->i_block[bNum] = allocate(f, -1);
            writeInode(f, iNum, i);
        }

        blockList = i->i_block;
        goto direct;
    } else if (bNum < 12 + k) {
        cout << "The block is in the first single indirect block in write" << endl;
        if (i->i_block == 0) {
            i->i_block[12] = allocate(f, -1);
            writeInode(f, iNum, i);
        }

        fetchBlock(f, i->i_block[12], temp);
        ibNum = i->i_block[12];
        blockList = temp;
        bNum -= 12;

        goto direct;
    } else if (bNum < 12 + k + k * k) {
        cout << "The block is in the first double indirect block in write" << endl;
        if (i->i_block[13] == 0) {
            i->i_block[13] = allocate(f, -1);
            writeInode(f, iNum, i);
        }

        fetchBlock(f, i->i_block[13], temp);

        ibNum = i->i_block[13];
        blockList = temp;
        bNum -= (12 + k);

        goto single;
    } else {
        cout << "The block is in the first triple indirect block in write" << endl;

        if (i->i_block[14] == 0) {
            i->i_block[14] = allocate(f, -1);
            writeInode(f, iNum, i);
        }

        fetchBlock(f, i->i_block[14], temp);

        ibNum = i->i_block[14];
        blockList = temp;

        long kPow = k * k;

        bNum -= (12 + k + kPow);

        index = bNum / (k * k);
        bNum = bNum % kPow;
    }

    single:
    index = bNum / k;
    bNum = bNum % k;

    if (blockList[index] == 0) {
        blockList[index] = allocate(f, -1);
        writeBlock(f, bNum, i);
    }
    ibNum = blockList[index];
    fetchBlock(f, blockList[index], temp);
    blockList = temp;

    direct:
    if (blockList[bNum] == 0) {
        blockList[bNum] = allocate(f, -1);
        writeBlock(f, bNum, i);
    }

    cout << "Made it to writeBlock" << endl;
    //writeBlock(f, blockList[bNum], buf);

    return 0;
}

int32_t allocate(Ext2File* f, int32_t blockGroup) {
    int32_t firstGroup = 0;
    int32_t lastGroup = f->num_block_groups -1;
    uint8_t* tempBlock = new uint8_t[f->file_system_block_size];
    uint32_t blockNum;

    if (blockGroup >= 0) {
        firstGroup = blockGroup;
        lastGroup = blockGroup;
    }

    for (int i = firstGroup; i <= lastGroup; i++) {
        fetchBlock(f, f->bgdt->blockGroups[i].bg_inode_bitmap, tempBlock);

        for (int j = 0; j < f->file_system_block_size; j++) {
            if (tempBlock[j] != 0xff) {
                for (int k = 0; k < 8; k++) {
                    if (tempBlock[j] & (1 << k)) {
                        blockNum = k + 8 * j + i * f->superBlock->s_blocks_per_group + f->superBlock->s_first_data_block;
                        tempBlock[j] |= 1 << k;
                        writeBlock(f, f->bgdt->blockGroups[i].bg_inode_bitmap, tempBlock);
                        f->bgdt->blockGroups[i].bg_free_blocks_count - 1;
                        f->superBlock->s_free_blocks_count - 1;
                        writeSuperblock(f, 0, f->superBlock);
                        writeBGDT(f, 0, f->bgdt);
                        return blockNum;
                    }
                }
            }
        }
    }

    return 0;
}
