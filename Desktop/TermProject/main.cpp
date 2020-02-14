#include <iostream>
#include "DisplayBuffer.h"
#include "VDIFile.h"
#include <iomanip>

using namespace std;

int main()
{
    uint8_t* pageBuffer = new uint8_t[64];
    VDIFile *f = vdiOpen("/home/csis/5806.public/Test-fixed-1k.vdi");

    cout << "fileDirector: " << f -> fd << endl;
    // To check the VDI Translation Map
    // displayBuffer((uint8_t *)f->map, f->header.cBlocks * sizeof(int), f -> header.offBlocks);

    // cout << "The cursor before vdiSeek: " << f -> cursor << endl;
    // vdiSeek(f, 446, SEEK_SET);
    // cout << "The cursor after vdiSeek: " << f -> cursor << endl;
    int read = vdiRead(f, pageBuffer, 64);
    cout << "The amount of read bytes: " << read << endl;
    // displayBufferPage(pageBuffer,64,190,256);
    displayBufferPage(pageBuffer, 64, 190, 256);
    vdiClose(f);


    return 0;
}
