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

uint32_t copyFromHost(Ext2File* f, char* fileToReadFrom) {
    int fd = open(fileToReadFrom, O_RDONLY, 0700);

    uint32_t newInum = allocateInode(f, -1);
    Inode newInode;
    memset(&newInode, 0, sizeof(Inode));
    newInode.i_atime = time(NULL);
    newInode.i_ctime = time(NULL);
    newInode.i_mtime = time(NULL);
    newInode.i_uid = 1000;
    newInode.i_gid = 1000;
    newInode.i_links_count = 1;
    newInode.i_mode = 0100644;

    uint32_t counter = 0;
    uint32_t bytesRead;
    uint32_t totalBytesRead = 0;
    uint8_t* pageBuffer = new uint8_t[f->file_system_block_size];
    writeInode(f, newInum, &newInode);
    while((bytesRead = read(fd, pageBuffer, f->file_system_block_size)) > 0) {
        writeBlockToFile(f, &newInode, counter, pageBuffer, newInum);
        counter++;
        totalBytesRead += bytesRead;
    }
    cout << "totalBytesRead: " << totalBytesRead << endl;
    newInode.i_size = totalBytesRead;
    writeInode(f, newInum, &newInode);

    cout << "new Inum: " << newInum << endl;

    Directory* d = openDir(f, 2);
    uint32_t lastDirentInum;
    char tempName[256];
    while (getNextDirent(f, d, lastDirentInum, tempName));
    cout << "Tempname: " << tempName << endl;
    cout << "Last dirent Inum: " << lastDirentInum << endl;
    uint32_t lastRecordLength = d->dirent->recLen;

    uint32_t neededLastRecordLength = (11 + d->dirent->nameLen) & ~3;

    char* baseName = basename(fileToReadFrom);

    uint32_t neededNewRecordLength = (11 + strlen(baseName)) & ~3;

    if (lastRecordLength >= neededLastRecordLength + neededNewRecordLength) {
        d->cursor -= d->dirent->recLen;
        d->dirent->recLen = neededLastRecordLength;
        d->cursor += d->dirent->recLen;
        d->dirent = (Dirent*)(d->block + d->cursor);
        d->dirent->iNum = newInum;
        for (int i = 0; i < strlen(baseName); i++) {
            d->dirent->name[i] = baseName[i];
        }
        d->dirent->nameLen = strlen(baseName);
        d->dirent->fileType = 1;
        d->dirent->recLen = f->file_system_block_size - d->cursor;
        writeBlockToFile(f, &d->iNode, 0, d->block, d->iNodeNum);
        uint8_t* buffer = new uint8_t[1024];
        fetchBlockFromFile(f, &d->iNode, 0, buffer);
        displayBuffer(buffer, 1024, 0);

        d->iNode.i_links_count++;
        writeInode(f, d->iNodeNum, &d->iNode);
    }

    close(fd);
}
