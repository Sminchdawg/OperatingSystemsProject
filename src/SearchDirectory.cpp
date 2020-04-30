#include "SearchDirectory.h"

using namespace std;

uint32_t searchDirectory (Ext2File* f, uint32_t iNum, char* name) {
    Directory* d = openDir(f, iNum);
    char tempName[256];
    while (getNextDirent(f, d, iNum, tempName)) {
        cout << tempName << endl;
        if (strcmp(name, tempName) == 0) {
            cout << "-----The directory is found-----" << endl;
            return d->dirent->iNum;
            break;
        }
    }
    return 0;
}

uint32_t searchForFile(Ext2File* f, char* name) {
    char * pch;
    pch = strtok (name,"/");
    uint32_t iNum = searchDirectory(f, 2, &pch[0]);
    pch = strtok (NULL, "/");
    while (pch != NULL)
    {
        iNum = searchDirectory(f, iNum, pch);
        pch = strtok (NULL, "/");
    }
    return iNum;
}
