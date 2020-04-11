#include <iostream>
#include "DisplayBuffer.h"
#include "VDIFile.h"
#include "PartitionFile.h"
#include <iomanip>

using namespace std;

int main()
{
    uint8_t* pageBuffer = new uint8_t[1024];
    VDIFile *f = vdiOpen("/home/csis/5806.public/Test-dynamic-1k.vdi");


    PartitionEntry partitionEntry[4];
    vdiSeek(f, 446, SEEK_CUR);
    vdiRead(f, partitionEntry, 64);

    for (int i = 0; i < 4; i++){
        cout << "Partition " << i + 1 << endl;
        printPartitionEntry(partitionEntry[i]);
        cout << endl << endl;
    }

    // Temp hack
    vdiSeek(f, -446, SEEK_CUR);

    /*
    Write stuff
    PartitionFile* partitionFile = partitionOpen(f, partitionEntry[0]);
    int seek = partitionSeek(partitionFile, 4048, SEEK_CUR);
    int write = partitionWrite(partitionFile, pageBuffer, 10);
    int read = partitionRead(partitionFile, pageBuffer, 10);

    Read stuff
    int seek = partitionSeek(partitionFile, 1024, SEEK_CUR);
    cout << "Cursor: " << partitionFile -> vdiFile -> cursor << endl;
    int read = partitionRead(partitionFile, pageBuffer, 1024);
    */


    displayBuffer(pageBuffer, 10, 0);
    partitionClose(partitionFile);
    return 0;
}
