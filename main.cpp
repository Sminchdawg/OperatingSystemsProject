#include <iostream>70
#include "DisplayBuffer.h"
#include "VDIFile.h"
#include "PartitionFile.h"
#include "Ext2File.h"
#include "Inode.h"
#include "FileAccess.h"
#include "DirectoryAccess.h"
#include <iomanip>

using namespace std;

int main()
{

    Ext2File* f = ext2Open("/home/csis/5806.public/Test-fixed-1k.vdi", -1);
    /*
    Inode* inode = new Inode;
    fetchInode(f, 0, inode);
    displayInode(inode);
    uint8_t* pageBuffer = new uint8_t[f->file_system_block_size];
    writeBlockToFile(f, inode, 0, pageBuffer);

    uint8_t* newPageBuffer = new uint8_t[f->file_system_block_size];
    fetchBlockFromFile(f, inode, 65804, newPageBuffer);
    displayBuffer(newPageBuffer, 256, 0);
    */

    char name[256];
    uint32_t iNum;
    Directory *d;

    cout << "Root directory contents" << endl;
    d = openDir(f, 2);
    while (getNextDirent(d, iNum, name)) {
        cout << "Inode: " << iNum << " name: " << d->block << endl;
    }

    closeDir(d);



    ext2Close(f);

    return 0;
}
