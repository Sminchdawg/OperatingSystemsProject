#include <iostream>
#include "DisplayBuffer.h"
#include "VDIFile.h"
#include "PartitionFile.h"
#include <iomanip>

using namespace std;

int main()
{
    // uint8_t* pageBuffer = new uint8_t[64];
    VDIFile *f = vdiOpen("/home/csis/5806.public/Test-dynamic-1k.vdi");
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

    PartitionEntry partitionEntry[4];
    vdiSeek(f, 446, SEEK_CUR);
    vdiRead(f, partitionEntry, 64);
    // partitionOpen(f, partitionEntry);

    for (int i = 0; i < 4; i++){
        cout << "Partition " << i + 1 << endl;
        printPartitionEntry(partitionEntry[i]);
        cout << endl << endl;
    }

    vdiClose(f);
    return 0;
}
