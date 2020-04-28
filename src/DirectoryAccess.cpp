#include "DirectoryAccess.h"

struct Directory *openDir(struct Ext2File *f, uint32_t iNum) {
    struct Directory* d = new Directory;
    d->block = new uint8_t[f->file_system_block_size];
    Inode* iNode = new Inode;
    fetchInode(f, iNum, iNode);
    displayInode(iNode);

    fetchBlockFromFile(f, iNode, 0, d->block);

    return d;
    // Open the directory with the given inode number
    // Return a pointer to a dynamically allocated structure that holds relevant information for processing
}

bool getNextDirent(struct Directory *d, uint32_t &iNum, char *name) {
    if (d->cursor < 5) {
        d->cursor++;
        return true;
    }
    return false;
    // Fetch the next director entry in sequence
    // Fill in the inode number and name in the last two parameters
    // Advance file cursor to the next entry
    // Return true if the fetch succeeded, or false if you were at the end of the directory
}

void rewindDir(struct Directory *d) {
    // Reset the directory's cursor to 0
    d->cursor = 0;
}

void closeDir(struct Directory *d) {
    // Close the directory
    // Deallocate any dynamic memory used by the directory
}
