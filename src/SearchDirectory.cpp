#include "SearchDirectory.h"

using namespace std;

uint32_t searchDirectory (Ext2File* f, uint32_t iNum, char* name) {
    Directory* d = openDir(f, iNum);
    char tempName[256];
    while (getNextDirent(f, d, iNum, tempName)) {
        if (strcmp(name, tempName) == 0) {
            return d->dirent->iNum;
            break;
        }
    }
    return 0;
}
