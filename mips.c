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
    printf("%s : %d", "result from register function with a$0", getRegisterNumberValue("$a0"));
    //getRegisterNumberValue("$a0");
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

int getRegisterNumberValue(char *name) {
        if (strcmp(name, "$zero") == 0) return 0;
        else if (strcmp(name, "$at") == 0) return 1;
        else if (strcmp(name, "$v0") == 0) return 2;
        else if (strcmp(name, "$v1") == 0) return 3;
        else if (strcmp(name, "$a0") == 0) return 4;
        else if (strcmp(name, "$a1") == 0) return 5;
        else if (strcmp(name, "$a2") == 0) return 6;
        else if (strcmp(name, "$a3") == 0) return 7;
        else if (strcmp(name, "$t0") == 0) return 8;
        else if (strcmp(name, "$t1") == 0) return 9;
        else if (strcmp(name, "$t2") == 0) return 10;
        else if (strcmp(name, "$t2") == 0) return 11;
        else if (strcmp(name, "$t3") == 0) return 12;
        else if (strcmp(name, "$t4") == 0) return 13;
        else if (strcmp(name, "$t5") == 0) return 14;
        else if (strcmp(name, "$t6") == 0) return 15;
        else if (strcmp(name, "$t7") == 0) return 16;
        else if (strcmp(name, "$s0") == 0) return 17;
        else if (strcmp(name, "$s1") == 0) return 18;
        else if (strcmp(name, "$s2") == 0) return 19;
        else if (strcmp(name, "$s3") == 0) return 20;
        else if (strcmp(name, "$s4") == 0) return 21;
        else if (strcmp(name, "$s5") == 0) return 22;
        else if (strcmp(name, "$s6") == 0) return 23;
        else if (strcmp(name, "$s7") == 0) return 24;
        else if (strcmp(name, "$t8") == 0) return 25;
        else if (strcmp(name, "$t9") == 0) return 26;
        else if (strcmp(name, "$k0") == 0) return 27;
        else if (strcmp(name, "$k1") == 0) return 28;
        else if (strcmp(name, "$gp") == 0) return 29;
        else if (strcmp(name, "$sp") == 0) return 30;
        else if (strcmp(name, "$fp") == 0) return 31;
        else if (strcmp(name, "$ra") == 0) return 32;

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
