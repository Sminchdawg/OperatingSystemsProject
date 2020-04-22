#include <iostream>
#include "DisplayBuffer.h"
#include "VDIFile.h"
#include "PartitionFile.h"
#include "Ext2File.h"
#include "Inode.h"
#include <iomanip>

using namespace std;

int main()
{

    Ext2File* f = ext2Open("/home/csis/5806.public/Test-fixed-1k.vdi", -1);
    bool inUse = inodeInUse(f, 255);
    cout << "In use: " << inUse << endl;
    freeInode(f, 255);
    bool nowInUse = inodeInUse(f, 255);
    cout << "Now in use: " << nowInUse << endl;

    ext2Close(f);

    return 0;
}
