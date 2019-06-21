#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

static const char *asmFileExtension  = ".asm";
static const char *instructionTokenDelimiters = " ,()";

char* test(int times);
void intToBinaryString(char* string, int numberToConvert, int currentPosition);
int encodeRTypeInstruction(char *instruction, int funct);
int encodeITypeInstruction(char *instruction, int opcode);
int getInstructionFunctValue(char *instruction);
int customExit(char *message);
int encodeInstruction(char *instruction);
int getInstructionHexadecimalValue(char * instruction);
int getRegisterNumberValue(char *name);
int getInstructionFunctValue(char *instruction);

int main(int argc, char ** argv) {
    if (argc <= 1) {
        customExit("Error, no mips file was passed as an argument to be encoded");
    }
    FILE *mipsFilePointer = fopen(argv[1],"r");
    if (mipsFilePointer == NULL) {
        customExit("Error while opening MIPS file");    
    }
    FILE *asmFilePointer = fopen(strcat(argv[1], asmFileExtension), "wb");
    if (asmFilePointer == NULL) {
        customExit("Error while opening ASM file");
    }
    char ch;
    int lineLengthCounter = 0;
    char * oneInstruction = (char*)calloc(32, sizeof(char));
    while ((ch = fgetc(mipsFilePointer)) != EOF) {
        
        if (ch == '\n') {
            oneInstruction[lineLengthCounter] = '\0';
            lineLengthCounter = 0;
            int encodedInstruction = encodeInstruction(oneInstruction);
            fwrite(&encodedInstruction, 1, sizeof(encodedInstruction), asmFilePointer);
            
        } else {
            oneInstruction[lineLengthCounter] = (char) ch;
            lineLengthCounter++;
        }   
    }
    fclose(asmFilePointer);
    fclose(mipsFilePointer);
    return 0;
}

int customExit(char * message) {
    printf("%s\n", message);
    exit(1);
}

int encodeRTypeInstruction(char *instruction, int funct) {

    char *nextToken;

    int rs, rt, rd;
    rd = getRegisterNumberValue(strtok(NULL, instructionTokenDelimiters));
    rs = getRegisterNumberValue(strtok(NULL, instructionTokenDelimiters));
    rt = getRegisterNumberValue(strtok(NULL, instructionTokenDelimiters));

    return rd << 11 | rs << 21 | rt << 16 | funct;

}

int encodeITypeInstruction(char *instruction, int opcode) {

    int rs, rt, immediateValue;

    char * firstToken = strtok(NULL,instructionTokenDelimiters);
    char * secondToken = strtok(NULL,instructionTokenDelimiters);
    char * thirdToken = strtok(NULL,instructionTokenDelimiters);

    rt = getRegisterNumberValue(firstToken);
    if (secondToken[0] == '$') {

        rs = getRegisterNumberValue(secondToken);
        immediateValue = (int) strtol(thirdToken, (char **)NULL, 10);

    } else {

        immediateValue = (int) strtol(secondToken, (char **)NULL, 10);
        rs = getRegisterNumberValue(thirdToken);

    }

    return opcode << 26| rs << 21 | rt << 16 | immediateValue;
}

int encodeInstruction(char * instruction) {

    char instructionTokenDelimiters[] = " ,()";
    char *ptr = strtok(instruction, instructionTokenDelimiters);
    int opcode = getInstructionHexadecimalValue(ptr);

    if (opcode == 0) {
        
        return encodeRTypeInstruction(ptr, getInstructionFunctValue(ptr));

    } else if (opcode >= 0x03) {

        return encodeITypeInstruction(ptr, opcode);
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
    else return -1;
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
    else return -1;
    
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
        else return -1;
}

