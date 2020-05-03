#ifndef FILEACCESS_H
#include <iostream>
#include <iomanip>
#include "cmath";
#include "DisplayBuffer.h"
#include "Ext2File.h"
#define FILEACCESS_H

int32_t fetchBlockFromFile(struct Ext2File *f, struct Inode *i, uint32_t bNum, void *buf);
int32_t writeBlockToFile(struct Ext2File *f, struct Inode *i, uint32_t bNum, void *buf);
int32_t allocate(Ext2File *f, int32_t blockGroup);

#endif // FILEACCESS_H
