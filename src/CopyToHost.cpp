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
