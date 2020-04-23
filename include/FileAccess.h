#ifndef FILEACCESS_H
#include <iostream>
#include <iomanip>
#define FILEACCESS_H

int32_t fetchBlockFromFile(struct Inode *i, uint32_t bNum, void *buf);
int32_t writeBlockFromFile(struct Inode *i, uint32_t bNum, void *buf);

#endif // FILEACCESS_H