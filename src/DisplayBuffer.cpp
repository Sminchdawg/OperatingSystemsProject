#include "DisplayBuffer.h"
#include <iostream>

using namespace std;

void displayBufferPage(void *buf, uint32_t count, uint32_t start, uint64_t offset) {
    char hex[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};


    uint8_t* buffer = static_cast<uint8_t*>(buf);

    printf("Offset: 0x");
    printf("%02X", offset);
    printf("\n");
    printf("   ");

    for (int i = 0; i < 16; i++){
        printf("0");
        printf("%c", hex[i]);
        printf(" ");
    }
    printf("      0...4...8...C\n");
    printf("  +-----------------------------------------------+  +----------------+\n");

    for (int i = 0; i < 16; i++){
        printf("0");
        printf("%c", hex[i]);
        printf("|");

        for (int j = 0; j < 16; j++) {
            // This is just for spacing
            if (j > 0) {
                printf(" ");
            }
                // This prints the hexadecmial values

            if (16 * i + j >= start && 16 * i + j < start + count) {
                printf("%02X", buffer[(16 * i + j) - start]);
            } else {
                printf("  ");
            }
        }
        printf("|");

        printf("0");
        cout << hex[i];
        printf("|");

        for (int j = 0; j < 16; j++) {

            if (16 * i + j >= start && 16 * i + j < start + count) {
                if (isprint(buffer[(16 * i + j) - start])) {
                    // This prints the characters values
                        printf("%c", buffer[(16 * i + j) - start]);
                    } else {
                        printf(" ");
                    }
            } else {
                printf(" ");
            }
        }

        printf("|\n");
    }

    printf("  +-----------------------------------------------+  +----------------+\n");
}


void displayBuffer(void *buf, uint32_t count, uint64_t offset) {
    int rounds = (count + 255)/ 256;
    // TODO: Maybe use newOffset? seems to work for now
    int newOffset = offset;

    for (int i = 0; i < rounds; i++) {
        int newCount = count - (256 * i);
        if (newCount < 256) {
            displayBufferPage(buf, newCount, 0, offset);
        } else {
            displayBufferPage(buf, 256, 0, offset);
        }
        offset += 256;
        buf+=256;
    }
}




