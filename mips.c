#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

char* test(int times);
void intToBinaryString(char* string, int numberToConvert, int currentPosition);
int encodeRTypeInstruction(char *instruction, int funct);
int getInstructionFunctValue(char *instruciton);

int main(int argc, char ** argv) {
    //bitsToInt(12345, 3, 5);
    //printf("%s\n", test(3));
    //char test[33];
    //intToBinaryString(test, 682, 31); 
    //printf("%s\n", test);
    //exit(0);
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
//            printLine(oneInstruction);
            encodeInstruction(oneInstruction);
            

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

int encodeRTypeInstruction(char *instruction, int funct) {
    //TODO: factoriser
    printf("calculated funct : %d\n", funct);
    char delim[] = " ,()"; // mettre en constante
    char *instructionChunk;
    
    if (funct == 0x00 || funct == 0x02) {
        //shift left logical and shift right logical
        int rd = (instructionChunk= strtok(NULL, delim))[0] == '$' ? getRegisterNumberValue(instructionChunk) : (int) strtol(instructionChunk, (char **)NULL, 10);
        int rt, shamt;
        rt = (instructionChunk= strtok(NULL, delim))[0] == '$' ? getRegisterNumberValue(instructionChunk) : (int) strtol(instructionChunk, (char **)NULL, 10);
        shamt = (instructionChunk= strtok(NULL, delim))[0] == '$' ? getRegisterNumberValue(instructionChunk) : (int) strtol(instructionChunk, (char **)NULL, 10);
        return rd << 11 | rt << 16 | shamt << 6 | funct
    } else if (funct == 0x001000) {
        //jump register
        int rs = (instructionChunk= strtok(NULL, delim))[0] == '$' ? getRegisterNumberValue(instructionChunk) : (int) strtol(instructionChunk, (char **)NULL, 10);
        return rs << 21 | funct;
    } else {
        int rs, rt;
        rs = (instructionChunk= strtok(NULL, delim))[0] == '$' ? getRegisterNumberValue(instructionChunk) : (int) strtol(instructionChunk, (char **)NULL, 10);
        rt = (instructionChunk= strtok(NULL, delim))[0] == '$' ? getRegisterNumberValue(instructionChunk) : (int) strtol(instructionChunk, (char **)NULL, 10);
        printf("calculated registers: %d %d %d\n", rd, rs, rt);
        return rd << 11 | rs << 21 | rt << 16 | funct;
    }
    
    
    
}

encodeInstruction(char * instruction) {
    int instructionChunk=0;
    char delim[] = " ,()";
    char *ptr = strtok(instruction, delim);
    int result;
    if (getInstructionHexadecimalValue(ptr) == 0) {
        //printf("this is a R encoded instruction: %s\n", ptr);
        
        result = encodeRTypeInstruction(ptr, getInstructionFunctValue(ptr));
    else {
        printf("this is not a R encoded instruction: %s\n", ptr);
    }
}

void intToBinaryString(char *string, int numberToConvert, int currentPosition) {
    
    if (currentPosition < 0) {
        string[32] = '\0';
        return;
    } 
    double powerOf2 = pow(2, currentPosition);
    if (numberToConvert >= powerOf2) {
        numberToConvert -= powerOf2;
        string[31 - currentPosition] = '1';
    } else {
        string[31 - currentPosition] = '0';
    }
    intToBinaryString(string, numberToConvert, --currentPosition);

}

int32_t bitsToInt(int numberToConvert, int startingBit, int size) {
    int32_t result=0;
    char numberToString[size+1];
    snprintf(numberToString, sizeof(numberToString), "%d", numberToConvert);
    int i;
    for (i=0; i<size;i++) {
        if (numberToString[i] == '1') {
            printf("%f\n",pow(2, startingBit));
        }
        startingBit++;
    }
}

int getInstructionFunctValue(char *instruction) {

    if (strcmp(instruction, "add") == 0) return 0x20;
    else if (strcmp(instruction, "addu") == 0)  return 0x21;
    else if (strcmp(instruction, "and") == 0)  return 0x24;
    else if (strcmp(instruction, "jr") == 0)  return 0x08;
    else if (strcmp(instruction, "nor") == 0)  return 0x27;
    else if (strcmp(instruction, "or") == 0)  return 0x25;
    else if (strcmp(instruction, "sll") == 0)  return 0x00;
    else if (strcmp(instruction, "slt") == 0)  return 0x2a;
    else if (strcmp(instruction, "sltu") == 0)  return 0x2b;
    else if (strcmp(instruction, "srl") == 0)  return 0x02;
    else if (strcmp(instruction, "sub") == 0)  return 0x22;
    else if (strcmp(instruction, "subu") == 0)  return 0x23;

}

int getInstructionHexadecimalValue(char * instruction) {
    
    if (strcmp(instruction, "add") == 0) return 0x00;
    else if (strcmp(instruction, "addi") == 0)  return 0x08; //funct 0x20
    else if (strcmp(instruction, "addiu") == 0)  return 0x09;
    else if (strcmp(instruction, "addu") == 0)  return 0x00; //funct 0x21
    else if (strcmp(instruction, "and") == 0)  return 0x00; //funct 0x24
    else if (strcmp(instruction, "andi") == 0)  return 0x0c;
    else if (strcmp(instruction, "beq") == 0)  return 0x04;
    else if (strcmp(instruction, "bne") == 0)  return 0x05;
    else if (strcmp(instruction, "j") == 0)  return 0x02;
    else if (strcmp(instruction, "jal") == 0)  return 0x03;
    else if (strcmp(instruction, "jr") == 0)  return 0x00; //funct 0x08
    else if (strcmp(instruction, "lb") == 0)  return 0x20;
    else if (strcmp(instruction, "lbu") == 0)  return 0x24;
    else if (strcmp(instruction, "lh") == 0)  return 0x21;
    else if (strcmp(instruction, "lhu") == 0)  return 0x25;
    else if (strcmp(instruction, "lui") == 0)  return 0x0f;
    else if (strcmp(instruction, "lw") == 0)  return 0x23;
    else if (strcmp(instruction, "nor") == 0)  return 0x00; //funct 0x27
    else if (strcmp(instruction, "or") == 0)  return 0x00; //funct 0x25
    else if (strcmp(instruction, "ori") == 0)  return 0x0d;
    else if (strcmp(instruction, "sb") == 0)  return 0x28;
    else if (strcmp(instruction, "sh") == 0)  return 0x29;
    else if (strcmp(instruction, "sll") == 0)  return 0x00; // funct 0x00
    else if (strcmp(instruction, "slt") == 0)  return 0x00; //funct 0x2a
    else if (strcmp(instruction, "slti") == 0)  return 0x0a;
    else if (strcmp(instruction, "sltiu") == 0)  return 0x0b;
    else if (strcmp(instruction, "sltu") == 0)  return 0x00; //funct 0x2b
    else if (strcmp(instruction, "srl") == 0)  return 0x00; // funct 0x02
    else if (strcmp(instruction, "sub") == 0)  return 0x00; // funct 0x22
    else if (strcmp(instruction, "subu") == 0)  return 0x00; //funct 0x23
    else if (strcmp(instruction, "sw") == 0)  return 0x2b;
    
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
        else if (strcmp(name, "$t3") == 0) return 11;
        else if (strcmp(name, "$t4") == 0) return 12;
        else if (strcmp(name, "$t5") == 0) return 13;
        else if (strcmp(name, "$t6") == 0) return 14;
        else if (strcmp(name, "$t7") == 0) return 15;
        else if (strcmp(name, "$s0") == 0) return 16;
        else if (strcmp(name, "$s1") == 0) return 17;
        else if (strcmp(name, "$s2") == 0) return 18;
        else if (strcmp(name, "$s3") == 0) return 19;
        else if (strcmp(name, "$s4") == 0) return 20;
        else if (strcmp(name, "$s5") == 0) return 21;
        else if (strcmp(name, "$s6") == 0) return 22;
        else if (strcmp(name, "$s7") == 0) return 23;
        else if (strcmp(name, "$t8") == 0) return 24;
        else if (strcmp(name, "$t9") == 0) return 25;
        else if (strcmp(name, "$k0") == 0) return 26;
        else if (strcmp(name, "$k1") == 0) return 27;
        else if (strcmp(name, "$gp") == 0) return 28;
        else if (strcmp(name, "$sp") == 0) return 39;
        else if (strcmp(name, "$fp") == 0) return 30;
        else if (strcmp(name, "$ra") == 0) return 31;

}

