#include "FileAccess.h"

int32_t fetchBlockFromFile(struct Inode *i, uint32_t bNum, void *buf) {
    // Read block bNum from the file, placing the data in the given buffer
}

int32_t writeBlockFromFile(struct Inode *i, uint32_t bNum, void *buf) {
    // Write the given buffer into the block bNum in the file
    // this may involve allocating one or more data blocks
}
