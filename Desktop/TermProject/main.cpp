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
    /*
    cout << "Count: " << f->superBlock->s_inodes_count << endl;
    cout << "Per group: " << f->superBlock->s_inodes_per_group << endl;
    cout << "Inode table?: " << f->bgdt->blockGroups[0].bg_inode_table << endl;
    cout << "Inode bitmap?: " << f->bgdt->blockGroups[0].bg_inode_bitmap << endl;
    cout << "Inode size: " << f->superBlock->s_inode_size << endl;
    */
    cout << endl;
    struct Inode* iNode = new Inode;
    fetchInode(f, 1, iNode);
    cout << endl;
    displayInode(iNode);

    ext2Close(f);

    return 0;
}
