#include "CopyToHost.h"

using namespace std;


uint32_t copyToHost(Ext2File* f, char* fileToReadFrom, char* fileToWriteTo) {

    uint32_t iNum = searchForFile(f, fileToReadFrom);
    Inode* inode = new Inode;
    fetchInode(f, iNum, inode);
    uint8_t* pageBuffer = new uint8_t[f->file_system_block_size];
    uint32_t numBlocks = inode->i_size / f->file_system_block_size + 1;
    uint32_t extra = inode->i_size % f->file_system_block_size;
    int fd = open(fileToWriteTo,  O_CREAT | O_WRONLY, 0666);

    int writeSize = f->file_system_block_size;
    for (int i = 0; i < numBlocks; i++) {
        fetchBlockFromFile(f, inode, i, pageBuffer);
        if (i + 1 == numBlocks) {
            writeSize = extra;
        }
        int w = write(fd, pageBuffer, writeSize);
    }

    close(fd);
    return 0;
}

uint32_t copyFromHost(Ext2File* f, char* fileToReadFrom, char* fileToWriteTo) {
    int fd = open(fileToReadFrom, O_RDONLY, 0700);
    uint32_t byteCount = lseek(fd, 0, SEEK_END);
    uint8_t* pageBuffer = new uint8_t[byteCount];
    lseek(fd, 0, SEEK_SET);

    int bytesRead = read(fd, pageBuffer, byteCount);

    uint32_t newInum = allocateInode(f, -1);
    cout << "new Inum: " << newInum << endl;

    uint32_t lastDirentInum;
    Directory* d = openDir(f, 2);
    char tempName[256];
    while (getNextDirent(f, d, lastDirentInum, tempName)) {}

    cout << "dir cursor: " << d->cursor << endl;
    cout << "lastDirentInum: " << lastDirentInum << endl;
    Dirent* dirent = new Dirent;
    dirent->fileType = 01;
    dirent->iNum = newInum;

    char* baseName = basename(fileToReadFrom);
    uint8_t baseNameSize = strlen(baseName);
    for (int i = 0; i < baseNameSize; i++) {
        dirent->name[i] = baseName[i];
    }

    dirent->nameLen = baseNameSize;
    dirent->recLen = f->file_system_block_size - (dirent->nameLen + 8);


    writeBlockToFile(f, &d->iNode, 1, dirent, dirent->iNum);
    uint8_t* buffer = new uint8_t[byteCount];
    fetchBlockFromFile(f, &d->iNode, 1, buffer);
    d->iNode.i_links_count++;
    displayBuffer(buffer, 256, 0);
    //d->iNode.i_size += 1024;
    displayInode(&d->iNode);
    writeInode(f, d->iNodeNum, &d->iNode);

    close(fd);
}
