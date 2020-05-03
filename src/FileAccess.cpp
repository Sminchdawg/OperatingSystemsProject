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
    } else if (bNum < 12 + k + pow(k, 2)) {
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

            long kPow = pow(k,2);

            bNum -= (12 + k + kPow);

            index = bNum / pow(k, 2);
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

int32_t writeBlockToFile(struct Ext2File *f, struct Inode *i, uint32_t bNum, void *buf) {
    // Write the given buffer into the block bNum in the file
    // this may involve allocating one or more data blocks
    int k = f->file_system_block_size / 4;

    uint32_t* blockList = new uint32_t[f->file_system_block_size / 4];
    uint32_t* temp = new uint32_t[f->file_system_block_size / 4];
    uint32_t ibNum;
    int index;

    if (bNum < 12) {
        if (i->i_block[bNum] == 0) {
            //i->i_block;
            // i_block[bNum] = bNum; // allocate
            // writeInode(f, iNum, buf); // figure out what is being written here
        } else {
            blockList = i->i_block;
            goto direct;
        }
    } else if (bNum < 12 + k) {
        cout << "The block is in the first single indirect block in write" << endl;
        if (i->i_block == 0) {
            // i_block[12] // allocate
            // writeInode(f, iNum, buf); // figure out what is being witten here
        } else {
            fetchBlock(f, i->i_block[12], temp);
            ibNum = i->i_block[12];
            blockList = temp;
            bNum -= 12;

            goto direct;
        }
    } else if (bNum < 12 + k + pow(k, 2)) {
        cout << "The block is in the first double indirect block in write" << endl;
        if (i->i_block[13] == 0) {
            // i_block[13] // allocate
            // writeInode(f, iNum, buf); // figure out what is being witten here
        } else {
            fetchBlock(f, i->i_block[13], temp);

            ibNum = i->i_block[13];
            blockList = temp;
            bNum -= (12 + k);

            goto single;
        }
    } else {
        cout << "The block is in the first triple indirect block in write" << endl;

        if (i->i_block[14] == 0) {
            // i_block[14] // allocate
            // writeInode(f, iNum, buf); // figure out what is being witten here
        } else {
            fetchBlock(f, i->i_block[14], temp);

            ibNum = i->i_block[14];
            blockList = temp;

            long kPow = pow(k,2);

            bNum -= (12 + k + kPow);

            index = bNum / pow(k, 2);
            bNum = bNum % kPow;
        }
    }

    single:
    index = bNum / k;
    bNum = bNum % k;

    if (blockList[index] == 0) {
        // blockList[index] // allocate
        // writeBlock(f, bNum, buf);
    } else {
        ibNum = blockList[index];
        fetchBlock(f, blockList[index], temp);
        blockList = temp;
    }

    direct:
    if (blockList[bNum] == 0) {
        // blockList[bNum] // allocate
        // writeBlock(f, bNum, buf);
    } else {
        cout << "Made it to writeBlock" << endl;
        writeBlock(f, blockList[bNum], buf);
    }

    return 0;
}

int32_t allocate(Ext2File* f, uint32_t blockGroup) {
/*
    uint32_t blockNum;
    f->superBlock->s_free_blocks_count--;
    f->bgdt[0]->blockGroups.bg_free_blocks_count--;
    writeSuperblock(f, 0, f->superBlock);
    writeBGDT(f, 0, f->bgdt[0]->blockGroups);
    return blockNum;
    */
}
