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
    // bool inUse = inodeInUse(f, 256);
    // cout << "In use: " << inUse << endl;
    //freeInode(f, 256);
    bool nowInUse = inodeInUse(f, 70);
    cout << "Now in use: " << nowInUse << endl;
    // freeInode(f, 1);
    int iNum = allocateInode(f, -1);
    cout << "The allocated iNum: " << iNum << endl;
    /*
    bool inUse = inodeInUse(f, 1);
    cout << "In use: " << inUse << endl;
    */
    ext2Close(f);

    return 0;
}
