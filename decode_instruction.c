#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char ** argv) {
    int counter = 0;
    unsigned int buffer;
//    unsigned int buffer;
    FILE * filePointer = fopen(argv[1], "rb");
//    while (fread(&buffer, 1, 1, filePointer) == 1) {
    while (fread(&buffer, sizeof(unsigned int), 1, filePointer) == 1) {
        printf("%x \n", buffer);
        counter ++;
    }
    printf("%d unsigned ints read \n", counter);
}
