#include <iostream>70
#include "DisplayBuffer.h"
#include "VDIFile.h"
#include "PartitionFile.h"
#include "Ext2File.h"
#include "Inode.h"
#include "FileAccess.h"
#include "DirectoryAccess.h"
#include "SearchDirectory.h"
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
    char* name = "examples/08.Strings";

    uint32_t iNum = searchDirectory(f, 30482, name);
    cout << iNum << endl;

  char * pch;
  pch = strtok (name," ,.-");
  while (pch != NULL)
  {
    printf ("%s\n",pch);
    pch = strtok (NULL, " ,.-");
  }
    ext2Close(f);

    return 0;
}
