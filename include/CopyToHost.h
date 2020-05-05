#ifndef COPYTOHOST_H
#include <iostream>
#include <iomanip>
#include "Ext2File.h"
#include "SearchDirectory.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>
#define COPYTOHOST_H

uint32_t copyToHost(Ext2File* f, char* fileToReadFrom, char* fileToWriteTo);
uint32_t copyFromHost(Ext2File* f, char* fileToReadFrom);

#endif // COPYTOHOST_H
