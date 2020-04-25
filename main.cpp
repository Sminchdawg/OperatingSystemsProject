#include <iostream>
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
    fetchInode(f, 2, inode);
    uint8_t* pageBuffer = new uint8_t[f->file_system_block_size];
    fetchBlockFromFile(f, inode, 0, pageBuffer);
    ext2Close(f);

    return 0;
}
