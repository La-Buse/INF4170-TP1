#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv) {
    if (argc <= 1) {
        customExit("Error, no mips file was passed as an argument to be encoded");
    }
    FILE *mipsFilePointer = fopen(argv[1],"r");
    if (mipsFilePointer == NULL) {
        customExit("Error while opening file");    
    }
    char ch;
    int lineLengthCounter = 0;
    char * oneInstruction = (char*)calloc(32, sizeof(char));
    while ((ch = fgetc(mipsFilePointer)) != EOF) {
        //printf("%c", ch);
        
        if (ch == '\n') {
            //printf("%s of length %d", "end of line!!!", lineLengthCounter);
            oneInstruction[lineLengthCounter] = '\0';
            lineLengthCounter = 0;
            printLine(oneInstruction);
        } else {
            oneInstruction[lineLengthCounter] = (char) ch;
            lineLengthCounter++;
        }

    }
    fclose(mipsFilePointer);
    return 0;
}

printLine(char * line) {
   printf("%s\n",line); 
}

customExit(char * message) {
    printf("%s\n", message);
    exit(1);
}

/*public char ** getMipsInstructionsFromFile(char * filePath) {
    //
    FILE *fp;
    fp = fopen(filePath, "r");
    if (fp != NULL) {
        
    } else {
        printf("Error while opening file\n");
        exit(1);
    }

}*/
