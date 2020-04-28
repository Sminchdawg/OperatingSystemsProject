#include <iostream>70
#include "DisplayBuffer.h"
#include "VDIFile.h"
#include "PartitionFile.h"
#include "Ext2File.h"
#include "Inode.h"
#include "FileAccess.h"
#include <iomanip>

using namespace std;

int main()
{

    Ext2File* f = ext2Open("/home/csis/5806.public/Test-fixed-1k.vdi", -1);
    Inode* inode = new Inode;
    fetchInode(f, 12, inode);
    displayInode(inode);
    uint8_t* pageBuffer = new uint8_t[f->file_system_block_size];
    fetchBlockFromFile(f, inode, 90380, pageBuffer);
    // fetchBlock(f, 534, pageBuffer);
    // displayBuffer(pageBuffer, 256, 0);
    ext2Close(f);

    return 0;
}
