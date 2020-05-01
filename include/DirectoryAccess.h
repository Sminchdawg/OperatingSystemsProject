#ifndef DIRECTORYACCESS_H
#include <iostream>
#include <iomanip>
#include "Ext2File.h"
#include "Inode.h"
#include "FileAccess.h"
#include "cmath"
#include "DisplayBuffer.h"
#include <string.h>
#define DIRECTORYACCESS_H

struct Dirent {
    uint32_t iNum; // Bytes 0-3
    uint16_t recLen; // Bytes 4-5
    uint8_t nameLen; // Bytes 6
    uint8_t fileType; // Bytes 7
    uint8_t name[1]; // Bytes 8
};

struct Directory {
    uint32_t cursor;
    uint8_t* block;
    uint32_t iNodeNum;
    Inode iNode;
    Dirent* dirent;
};

struct Directory *openDir(struct Ext2File *f, uint32_t iNum);
bool getNextDirent(struct Ext2File *f, struct Directory *d, uint32_t &iNum, char *name);
void rewindDir(struct Directory *d);
void closeDir(struct Directory *d);
void getAllDirents (Ext2File* f, uint32_t iNum, char* tempName, int tabLevel);

#endif // DIRECTORYACCESS_H
