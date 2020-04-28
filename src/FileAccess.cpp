#include "FileAccess.h"
#include "Inode.h"

using namespace std;

int32_t fetchBlockFromFile(struct Ext2File *f, struct Inode *i, uint32_t bNum, void *buf) {
    // Read block bNum from the file, placing the data in the given buff

    // i_block in an inode is an array of 15 block numbers

    // first twelve entries of the i_block contain indexxes for the file's first twelve data blocks, indeces 0-11

    // index 12 doesnt have a block number for the next data block
    // It tells you where to find a single indirect block, which is a data block that's been sliced into 4-byte chunks,
    // each holding the block number of the next k data blocks, where k is the block size divdied by 4.
    // using a 1kb system k = 256

    // Index 13 holds the index of a double indirect lbock, which contains the indexes of k single indirect blocks,
    // each of which holds k indexes of data blocks, giving access to k^2 additional data blocks

    // Index 14 holds a triple indirect block, it holds the indexes of k double indirect blocks,
    // which each hold the indeces of k single indiret blocks, which each hold the indeces of k datat blocks

    // Observations
    // The index of every data block is held in an array, that arrayh might be i_block array or a single indirect block

    int k = f->file_system_block_size / 4;

    uint32_t* blockList = new uint32_t[f->file_system_block_size / 4];
    int index;
    // If the index is in the i_block array, since 0-11 indeces are a block number in the i_block array
    if (bNum < 12) {
        // This is never going to work kekw
        blockList = i->i_block;

        // Goes to direct
        goto direct;
    } else if (bNum < (12 + k)) {
        cout << "The block is in the first single indirect block" << endl;
        // ^^^ Index is in the first single indirect block, aka i_block[12]
        if (i->i_block == 0) {
            return false;
        } else {
            fetchBlock(f, i->i_block[12], blockList);
            // buf gets a signle indirect block, which is a data block thats been sliced into 4-byte chunks,
            // each holding the block number of the next k data blocks, where k is the block size divided by 4
            // so buf should have the block numbers of the next 256 data blocks
            // each 4 byte chunk of the block has 1 block number
            bNum -= 12;
            // Goes to direct
            goto direct;
        }
    } else if (bNum < 12 + k + pow(k, 2)) {
        cout << "The block is in the first double indirect block" << endl;
        // ^^ Index is in first double indirect block, aka i_block[13]
        if (i->i_block[13] == 0) {
            return false;
        } else {
            fetchBlock(f, i->i_block[13], blockList);
            // buf gets indexes of k single indirect blocks
            bNum -= (12 + k);
            cout << "bNum after sub: " << bNum << endl;
            // Goes to single
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
        cout << "Bnum: " << blockList[bNum] << endl;
        fetchBlock(f, blockList[bNum], buf);
        return true;
    }
}

int32_t writeBlockFromFile(struct Ext2File *f, struct Inode *i, uint32_t bNum, void *buf) {
    // Write the given buffer into the block bNum in the file
    // this may involve allocating one or more data blocks
    int k = f->file_system_block_size / 4;

    uint32_t* blockList = new uint32_t[f->file_system_block_size / 4];
    uint32_t* temp = new uint32_t[f->file_system_block_size / 4];
    uint32_t ibNum;
    int index;

    if (bNum < 12) {
        if (i->i_block[bNum] == 0) {
            // i_block[b]; // allocate
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

int32_t allocate() {
    return 0;
}
