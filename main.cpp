#include <iostream>
#include "DisplayBuffer.h"
#include "VDIFile.h"
#include "PartitionFile.h"
#include "Ext2File.h"
#include "Inode.h"
#include "FileAccess.h"
#include "DirectoryAccess.h"
#include "SearchDirectory.h"
#include "CopyToHost.h"
#include <iomanip>

using namespace std;

int main()
{

    Ext2File* f = ext2Open("/home/csis/5806.public/Test-fixed-4k.vdi", -1);


    int input;
    while (input != 4) {
        cout << "1. List all files in the directory" << endl;
        cout << "2. Write to the host" << endl;
        cout << "3. Write to the VDI" << endl;
        cout << "4. Exit" << endl;
        cin >> input;

        if (input == 1) {
            getAllDirents(f, 2, -1);
        } else if (input == 2) {
            char* fileToReadFrom = new char[256];
            char* fileToWriteTo = new char[256];
            cout << "Enter a directory to a file from the vdi file: ";
            cin >> fileToReadFrom;
            uint32_t num = searchDirectory(f, 2, fileToReadFrom);

            cout << "Enter a directory to write to: ";
            cin >> fileToWriteTo;
            copyToHost(f, fileToReadFrom, fileToWriteTo);
        } else if (input == 3) {
            char* fileToReadFrom = new char[256];
            cout << "Enter a directory to a file from the host: ";
            cin >> fileToReadFrom;
            copyFromHost(f, fileToReadFrom);
        }
    }


    // getAllDirents(f, 2, -1);

    ext2Close(f);

    return 0;
}
