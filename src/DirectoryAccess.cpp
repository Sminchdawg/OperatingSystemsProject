#include "DirectoryAccess.h"

using namespace std;

struct Directory *openDir(struct Ext2File *f, uint32_t iNum) {
    // Open the directory with the given inode number
    // Return a pointer to a dynamically allocated structure that holds relevant information for processing
    struct Directory* d = new Directory;
    d->block = new uint8_t[f->file_system_block_size];
    fetchInode(f, iNum, &d->iNode);
    d->iNodeNum = iNum;

    return d;
}

bool getNextDirent(struct Ext2File *f, struct Directory *d, uint32_t &iNum, char *name) {
    while (d->cursor < d->iNode.i_size) {
        int blockNum = d->cursor / f->file_system_block_size;
        int offset = d->cursor % f->file_system_block_size;
        fetchBlockFromFile(f, &d->iNode, blockNum, d->block);
        d->dirent = (Dirent*)(d->block + offset);
        d->cursor += d->dirent->recLen;
        if (d->dirent->iNum != 0) {
            iNum = d->dirent->iNum;
            for (int i=0;i<d->dirent->nameLen;i++) {
                name[i] = d->dirent->name[i];
            }
            name[d->dirent->nameLen] = 0;
            return true;
        }
    }

    return false;
}

void rewindDir(struct Directory *d) {
    // Reset the directory's cursor to 0
    d->cursor = 0;
}

void closeDir(struct Directory *d) {
    // Close the directory
    // Deallocate any dynamic memory used by the directory

}
