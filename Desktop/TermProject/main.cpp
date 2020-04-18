#include <iostream>
#include "DisplayBuffer.h"
#include "VDIFile.h"
#include "PartitionFile.h"
#include "Ext2File.h"
#include <iomanip>

using namespace std;

int main()
{
    uint8_t* pageBuffer = new uint8_t[1024];

    Ext2File* f = ext2Open("/home/csis/5806.public/Test-fixed-1k.vdi", -1);

    ext2Close(f);

    return 0;
}
