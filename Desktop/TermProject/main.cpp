#include <iostream>
#include "DisplayBuffer.h"
#include "VDIFile.h"
#include "PartitionFile.h"
#include <iomanip>

using namespace std;

int main()
{
    uint8_t* pageBuffer = new uint8_t[64];
    VDIFile *f = vdiOpen("/home/csis/5806.public/Test-fixed-1k.vdi");
    // To check the VDI Translation Map
    // displayBuffer((uint8_t *)f->map, f->header.cBlocks * sizeof(int), f -> header.offBlocks);
    /* STEP ONE DEBUGGING AND TESTING
    vdiSeek(f, 446, SEEK_SET);
    // vdiWrite(f, pageBuffer, 64);
    int read = vdiRead(f, pageBuffer, 64);
     // int write = vdiWrite(f, pageBuffer, 64);
    // dread = vdiRead(f, pageBuffer, 64);
    cout << "The amount of read bytes: " << read << endl;
    displayBufferPage(pageBuffer, 64, 190, 256);
    */
    PartitionEntry partitionEntry;
    partitionOpen(f, partitionEntry);


    vdiClose(f);


    return 0;
}
