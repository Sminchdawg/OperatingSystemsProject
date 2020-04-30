#ifndef COPYTOHOST_H
#include <iostream>
#include <iomanip>
#include "Ext2File.h"
#include "SearchDirectory.h"
#define COPYTOHOST_H

uint32_t copyToHost(Ext2File* f, char* fileToReadFrom, char* fileToWriteTo);

#endif // COPYTOHOST_H
