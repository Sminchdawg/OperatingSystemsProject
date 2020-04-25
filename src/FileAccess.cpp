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

    int k = i->i_size / 4;

    // If the index is in the i_block array, since 0-11 indeces are a block number in the i_block array
    if (bNum < 12) {
        int* blockList = new int[12];
        // This is never going to work kekw
        blockList = i->i_block;

        // Goes to direct
    } else if (bNum < (12 + k)) {
        // ^^^ Index is in the first single indirect block, aka i_block[12]
        if (i->i_block == 0) {
            return false;
        } else {
            fetchBlock(f, i_block[12], buf);
            // buf gets a signle indirect block, which is a data block thats been sliced into 4-byte chunks,
            // each holding the block number of the next k data blocks, where k is the block size divided by 4
            // so buf should have the block numbers of the next 256 data blocks
            // each 4byte chunk of the block has 1 block number?
            int* blockList = buf;

            bNum -= 12;
            // Goes to direct
        }
    } else if (bNum < 12 + k + pow(k, 2)) {
        // ^^ Index is in first double indirect block, aka i_block[13]
        if (i_block[13] == 0) {
            return false;
        } else {
            fetchBlock(f, i_block[13], buf);

            // buf gets indexes of k single indirect blocks
            int* blockList = buf;
            bNum -= b - 12 - k;

            // Goes to single
        }
    } else {
        // ^^ Index is in the first triple indirect block, aka i_block[14]
        if (i_block[14] == 0) {
            return false;
        } else {
            fetchBlock(f, i_block[14], buf)

            int* blockList = buf;
            b -= b - 12 - k - pow(k, 2);

            int index = b / pow(k, 2);
            bNum = b % pow(k,2);

            if (blockList[index] == 0) {
                return false;
            } else {
                fetchBlock(f, blockList[index], buf);

                int* blockList = buf;

                // Goes to single
            }
        }
    }

    // Single
    index = bNum / k;
    bNum = b % k;

    if (blockList[index] == 0) {
        return false;
    } else {
        fetchBlock(f, blockList[index], buf);

        int* blockList = buf;
    }

    // Direct
    if (blockList[bNum] == 0) {
        return false;
    } else {
        fetchBlock(f, blockList[bNum], buf);
        return true;
    }
}

int32_t writeBlockFromFile(struct Inode *i, uint32_t bNum, void *buf) {
    // Write the given buffer into the block bNum in the file
    // this may involve allocating one or more data blocks
}
