#include "VDIFile.h"
#include <iostream>
#define VDIFILE_H

using namespace std;

struct VDIFile *vdiOpen(char *fn) {
    struct VDIFile* f = new VDIFile;
    // Opens the file whose name is given
    // Sets the file descriptor whatever was returned from the open() system call
    f -> fd = open(fn, O_RDWR, 0600);

    if(f -> fd == -1) {
        cout << "Cannot open file" << endl;
        return NULL;
    }

    read(f -> fd, &(f -> header), sizeof(VDIHEADER1PLUS));

    f -> cursor = 0;

    f -> map = new uint32_t[f -> header.cBlocks];
    lseek(f -> fd, f -> header.offBlocks, SEEK_SET);
    read(f -> fd, f -> map, f -> header.cBlocks * sizeof(uint32_t));

    return f;
};

void vdiClose (struct VDIFile *f) {
    // Close the file whose pointer is given
    close(f -> fd);

    // Deallocate any dynamically created memory regions
    delete[] f -> map;
    delete f;
}

off_t vdiSeek(VDIFile *f, off_t offset, int anchor) {
    int newLocation = 0;

    if (anchor == SEEK_SET) {
        newLocation = offset; // offset
    } else if (anchor == SEEK_CUR) {
        newLocation = offset + f -> cursor; // offset + currentLocation
    } else if (anchor == SEEK_END) {
        newLocation = offset + f -> header.cbDisk; // offset plus max size
    } else {
        cout << "The anchor is invalid";
        return -1; // This is if the anchor is invalid, we must fail
    }

    if (newLocation >= 0 && newLocation < f -> header.cbDisk) {
        f -> cursor = newLocation;
    } else {
        cout << "The cursor was moved out of the disk space";
        return -1; // If the cursor is moved out of the diskSpace, we must fail
    }

    return f -> cursor;
}

ssize_t vdiRead(struct VDIFile *f, void *buf, size_t count) {
    cout << "Count me daddy: " << count << endl;
    int bytesLeft = count;
    int bytesToRead;
    int bytesRead = 0;
    int numLoops = 0;
    int vPage, pPage, offset;

    while (bytesLeft > 0) {
        // Finds the virtual page by taking the cursor and diving it by the size of the block ie: 700 / a block size of 300 would tell you that you are in block 2
        vPage = f -> cursor / f -> header.cbBlock;
        // Offset is found from taking the cursor and modding the block size ie: the cursor is 700 and the block size is 300, we know we are 100 bytes into that block
        offset = f -> cursor % f -> header.cbBlock;
        // Passes the virtual page into the map and finds the physical
        pPage = f -> map[vPage];
        cout << "Header: " << f -> header.cbHeader << endl;
        cout << "OffBlocks: " <<  f -> header.offBlocks << endl;
        cout << "OffData: " <<  f -> header.offData << endl;

        if (bytesLeft < f->header.cbBlock - offset) {
            bytesToRead = bytesLeft;
        } else {
            bytesToRead = f->header.cbBlock - offset;
        }


        int something = lseek(f -> fd,f -> header.offData + pPage * f -> header.cbBlock + offset, SEEK_SET);

        int misterReed = read(f -> fd, buf, bytesToRead);

       // Check to see if there was an error in trying to read
       if (misterReed == -1) {
            bytesRead = -1;
            cout << "There was an error reading the file";
            break;
       }

       bytesLeft -= bytesToRead;
       bytesRead += bytesToRead;
       numLoops++;
    }
    // Maybe????
    return bytesRead;
}

/*
ssize_t vdiWrite(struct VDIFile *f, void *buf, size_t count) {
    int bytesLeft = count;
    int bytesWrote = 0;
    int bytesToWrite;
    int numLoops = 0;

   // Figure out how many bytes to write, probably gonna be 256 or if there isnt 256 less than however many less
    while (bytesLeft > 0) {
        if (bytesLeft < 256) {
            bytesToWrite = bytesLeft;
        } else {
            bytesToWrite = 256;
        }

        // Figure out where to start on the page/block
        // Missing offset
       lseek(fileDescriptor, numLoops, SEEK_SET);
       int missedWrittens = write(fileDescriptor, buf, bytesToWrite);

       if (missedWrittens == -1) {
        bytesWrote = -1;
        break;
       }

       bytesLeft -= bytesToWrite;
       bytesWrote += bytesToWrite;
       numLoops++;
    }

    return bytesWrote;
}
*/
