#ifndef SEARCHDIRECTORY_H
#include "Ext2File.h"
#include <iostream>
#include <iomanip>
#include "DirectoryAccess.h"
#include <stdio.h>
#include <string.h>
#define SEARCHDIRECTORY_H

uint32_t searchDirectory (Ext2File* f, uint32_t iNum, char* name);


#endif // SEARCHDIRECTORY_H
