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

    Ext2File* f = ext2Open("/home/csis/5806.public/Test-fixed-1k.vdi", -1);

    char fileToReadFrom[] = "examples/08.Strings/StringComparisonOperators/StringComparisonOperators.txt";
    char fileToWriteTo[] = "/home/csis/stringComparisonOperators";
    copyToHost(f, fileToReadFrom, fileToWriteTo);

    ext2Close(f);

    return 0;
}
