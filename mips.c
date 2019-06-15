#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

char *asmFileExtension  = ".asm";

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
    printf("calculated funct : %d\n", funct);
    char delim[] = " ,()"; // mettre en constante
    char *nextToken;
    
    if (funct == 0x00 || funct == 0x02) {

        //shift left logical and shift right logical
        int rd = (nextToken= strtok(NULL, delim))[0] == '$' ? getRegisterNumberValue(nextToken) : (int) strtol(nextToken, (char **)NULL, 10);
        int rt = getRegisterNumberValue(strtok(NULL, delim));
        int shamt = (int) strtol(strtok(NULL, delim), (char **)NULL, 10);

        return rd << 11 | rt << 16 | shamt << 6 | funct;

    } else if (funct == 0x001000) {

        //jump register
        int rs = strtol(strtok(NULL, delim), (char **)NULL, 10);

        return rs << 21 | funct;

    } else {

        int rs, rt, rd;
        rd = getRegisterNumberValue(strtok(NULL, delim));
        rs = getRegisterNumberValue(strtok(NULL, delim));
        rt = getRegisterNumberValue(strtok(NULL, delim));

        return rd << 11 | rs << 21 | rt << 16 | funct;

    }
}

int encodeITypeInstruction(char *instruction, int opcode) {
    //addi  R[rt] = R[rs] + Imm,                                                         addi $s3,$s3,1,       # s3 = s3 + 1              OK
    //addiu R[rt] = R[rs] + Imm,                                                         addiu $s3,$s3,1       # s3 = s3 + 1              OK
    //andi  R[rt] = R[rs] & Imm,                                                         andi $s2,$t0,7,       # s2 = t0 & 7              OK
    //beq   if (R[rs] == R[rt]) then PC = PC + 4 + Imm,                                  beq $s1,$zero,Fin,    # si s1 == 0, on branche à l'étiquette Fin  NOT OK $s1 va dans rs, $zero dans rt 
    //bne   if (R[rs] != R[rt]) then PC = PC + 4 + Imm,                                  bne $s1,$zero,Fin,    # si s1 != 0, on branche à l'étiquette Fin  NOT OK
    //lb    R[rt] = 24*signe, M[R[rs] + Imm]                                             lb $s2,0($t2)                                    OK
    //lbu   R[rt] = 24*0, M[R[rs]] (moins significatif), des 0 dans le reste             lbu $s2,0($t2)                                   OK
    //lh    R[rt] = 16*signe, M[R[rs] + Imm]                                             lh $s2,0($t2)                                    OK
    //lhu   R[rt] = 16*0, M[R[rs]] (moins significatifs), des 0 dans le reste            lhu $s2,0($t2)                                   OK
    //lui   R[rt] = Imm (dans les 2 octets les plus significatifs), des 0 dans le reste  lui $s2,8             # s2 = 8 << 16             OK            
    //lw    R[rt] = M[R[rs] + Imm]                                                       lw $t0,32($s3)        # t0 = s3[8]               OK
    //ori   R[rt] = R[rs] | Imm                                                          ori $t0,$s5,7         # t0 = s5 | 7              OK
    //sb    M[R[rs] + Imm] = R[rt] (octet le moins significatif)                         sb $t1,0($s2)         # s2[0] = t1               OK
    //sh    M[R[rs] + Imm] = R[rt] (2 octets les moins significatifs)                    sh $t1,0($s2)         # s2[0] = t1               OK
    //slti  R[rt] = (R[rs] < Imm) ? 1 : 0                                                slti $t0,$s2,10       # t0 = (s2 < 10) ? 1 : 0   OK
    //stliu R[rt] = (R[rs] < Imm) ? 1 : 0                                                sltiu $t0,$s2,10      # t0 = (s2 < 10) ? 1 : 0   OK
    //sw    M[R[rs] + Imm] = R[rt]                                                       sw $t0,1200($t1)      # t1[300] = t0             OK
    //le premier token lu est toujours rt pour les instructions de type I
    //ensuite, si le deuxieme token lu est un est un registre, alors c'est rs et le dernier token est imm
    //sinon, le deuxieme token lu est imm est le dernier est rs

    char delim[] = " ,()"; // mettre en constante
    int rs, rt, immediateValue;

    char * firstToken = strtok(NULL,delim);
    char * secondToken = strtok(NULL,delim);
    char * thirdToken = strtok(NULL,delim);

    rt = getRegisterNumberValue(firstToken);
    if (firstToken[0] == '$') {

        rs = getRegisterNumberValue(secondToken);
        immediateValue = (int) strtol(thirdToken, (char **)NULL, 10);

    } else {

        immediateValue = (int) strtol(secondToken, (char **)NULL, 10);
        rs = getRegisterNumberValue(thirdToken);

    }

    return opcode << 26| rs << 21 | rt << 16 | immediateValue;
}

int encodeInstruction(char * instruction) {

    char delim[] = " ,()";
    char *ptr = strtok(instruction, delim);
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

