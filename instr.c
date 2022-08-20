#include "instr.h"

//  instruction functions
void decode1(uint16_t instr){
    switch ((instr & 0xf000) >> 12){
        case 0xf:       //  escape to decode 2
            n2(instr);
            break;

        case 0x0:       // addition
            //  set flags accordingly
            //  if the sum is zero
            if ((((int)(reg[(instr & 0x00f0) >> 4])) + ((int)(reg[instr & 0x000f]))) == 0x0){
                reg[21] = (reg[21] | 0x4000);
            }
            // if the sum is greater than 65535
            if ((((int)(reg[(instr & 0x00f0) >> 4])) + ((int)(reg[instr & 0x000f]))) > 0xffff){
                reg[21] = (reg[21] | 0x2000);
            }
            /*   TODO: signed integer overflow flag testing   */
            //  actually add the numbers
            reg[(instr & 0x0f00) >> 8] = reg[(instr & 0x00f0) >> 4] + reg[instr & 0x000f];
            break;

        case 0x1:       //  subtraction
            //  negetive flag test
            if ((((int)(reg[(instr & 0x00f0) >> 4])) - ((int)(reg[instr & 0x000f]))) < 0){
                reg[21] = (reg[21] | 0x8000);
            }
            //  zero-flag test
            if ((((int)(reg[(instr & 0x00f0) >> 4])) - ((int)(reg[instr & 0x000f]))) == 0x0){
                reg[21] = (reg[21] | 0x4000);
            }
            //  negetive flag test
            if (((int)(reg[(instr & 0x00f0) >> 4])) < ((int)(reg[instr & 0x000f]))){
                reg[21] = (reg[21] | 0x2000);
            }
            /*   TODO: signed integer carry flag test   */
            //  actually subtract the numbers
            reg[(instr & 0x0f00) >> 8] = reg[(instr & 0x00f0) >> 4] - reg[instr & 0x000f];
            break;

        case 0x2:       //  multiplication
            //  zero-flag test
            if ((((int)(reg[(instr & 0x00f0) >> 4])) * ((int)(reg[instr & 0x000f]))) == 0x0){
                reg[21] = (reg[21] | 0x4000);
            }
            // if the sum is greater than 65535
            if ((((int)(reg[(instr & 0x00f0) >> 4])) + ((int)(reg[instr & 0x000f]))) > 0xffff){
                reg[21] = (reg[21] | 0x2000);
            }
            /*   TODO: signed integer overflow flag testing   */
            //  actual multiplication
            reg[(instr & 0x0f00) >> 8] = reg[(instr & 0x00f0) >> 4] * reg[instr & 0x000f];
            break;

        case 0x3:       //  integer division
            //  zero flag test
            if (((int)(reg[(instr & 0x00f0) >> 4])) == 0 || ((int)(reg[instr & 0x000f])) == 0x0){
                reg[21] = (reg[21] | 0x4000);
                reg[(instr & 0xf00) >> 8] = 0x0000;
            } else {        //  TODO: signed integer division
                reg[(instr & 0xf00) >> 8] = reg[(instr & 0x00f0) >> 4] / reg[(instr & 0x000f)];
            }
            break;

        case 0x4:      //  remainder finder
            //  zero test
            if ((((int)(reg[(instr & 0x00f0) >> 4])) % ((int)(reg[instr & 0x000f]))) == 0x0){
                reg[21] = (reg[21] | 0x4000);
            }
            reg[(instr & 0x0f00) >> 8] = reg[(instr & 0x00f0) >> 4] % reg[instr & 0x000f];
            break;

        case 0x5:      //  AND
            reg[(instr & 0x0f00) >> 8] = reg[(instr & 0x00f0) >> 4] & reg[(instr & 0x000f)];
            if (reg[(instr & 0x0f00) >> 8] == 0){
                reg[21] = (reg[21] | 0x4000);
            }
            break;

        case 0x6:      //  NAND
            reg[(instr & 0x0f00) >> 8] = ~(reg[(instr & 0x00f0) >> 4] & reg[(instr & 0x000f)]);
            if (reg[(instr & 0x0f00) >> 8] == 0){
                reg[21] = (reg[21] | 0x4000); 
            }
            break;

        case 0x7:      //  OR
            reg[(instr & 0x0f00) >> 8] = reg[(instr & 0x00f0) >> 4] | reg[(instr & 0x000f)];
            if (reg[(instr & 0x0f00) >> 8] == 0){
                reg[21] = (reg[21] | 0x4000);
            }
            break;

        case 0x8:      //  AND
            reg[(instr & 0x0f00) >> 8] = ~(reg[(instr & 0x00f0) >> 4] | reg[(instr & 0x000f)]);
            if (reg[(instr & 0x0f00) >> 8] == 0){
                reg[21] = (reg[21] | 0x4000);
            }
            break;

        case 0x9:      //  XOR
            reg[(instr & 0x0f00) >> 8] = reg[(instr & 0x00f0) >> 4] ^ reg[(instr & 0x000f)];
            if (reg[(instr & 0x0f00) >> 8] == 0){
                reg[21] = (reg[21] | 0x4000);
            }
            break;

        case 0xa:      //  XNOR
            reg[(instr & 0x0f00) >> 8] = ~(reg[(instr & 0x00f0) >> 4] ^ reg[(instr & 0x000f)]);
            if (reg[(instr & 0x0f00) >> 8] == 0){
                reg[21] = (reg[21] | 0x4000);
            }
            break;

        case 0xb:       //  load direct from memory address (lower 256 words, first 4k bits)
            reg[(instr & 0x0f00) >> 8] = mem[(instr & 0x00ff)];
            break;

        case 0xc:       //  store direct from memory address (lower 256 words, first 4k bits)
            mem[(instr & 0x00ff)] = reg[(instr & 0x0f00) >> 8];
            break;

        case 0xd:       //  compare immediate; odd-numbered absolute jump targets
            reg[21] = reg[21] | 0x0800;
            cmp(instr);
            break;

        case 0xe:       //  set r1r1 to the value stored in the lower byte of the instruction
            reg[(instr & 0x0f00) >> 8] = (instr & 0x00ff);
            break;

        default:
            const char Error[] = "Error in decoder1; invalid number";
            break;
    }
    return;
}
void decode2(uint16_t instr){
    switch((instr & 0x0f00) >> 8){
        case 0x0:       //  escape to decode 3
            decode3(instr);
            break;
        
        case 0x1:      //  cmp command
            cmp (instr);
            break;

        case 0x2:       //  move
            reg[(instr & 0x00f0) >> 4] = reg[(instr & 0x000f)];
            break;

        case 0x3:       //  swap registers
            uint16_t temp = reg[(instr & 0x00f0) >> 4];
            reg[(instr & 0x00f0) >> 4] = reg[(instr & 0x000f)];
            reg[(instr & 0x000f)] = temp;
            break;

        case 0x4:       //  load indirect
            reg[(instr & 0x00f0)] = mem[reg[(instr & 0x000f)]];
            break;

        case 0x5:       //  store indirect
            mem[reg[(instr & 0x000f)]] = reg[(instr & 0x00f0) >> 4];
            break;

        default:
            const char Error[] = "Error in decoder2; invalid number";
            break;
    }
    return;
}
void decode3(uint16_t instr){
    switch((instr & 0x00f0) >> 4){
        case 0x0:       //  escape to decode4
            decode4(instr);
            break;

        case 0x1:       //  NOT
            reg[(instr & 0x000f)] = ~(reg[(instr & 0x000f)]);
            if (reg[(instr & 0x000f)] == 0x0){
                reg[21] = (reg[21] | 0x4000);
            }
            break;

        case 0x2:       //  increment
            if (((int)reg[(instr & 0x000f)]) == 65535){
                reg[21] = reg[21] | 0x6000;
            }
            reg[(instr & 0x000f)] = reg[(instr & 0x000f)] + 1;
            break;

        case 0x3:       //  decrement
            if (((int)reg[(instr & 0x000f)]) == 1){
                reg[21] = reg[21] | 0x4000;
            }
            if (((int)reg[(instr & 0x000f)]) == 0){
                reg[21] = reg[21] | 0x2000;
            }
            reg[(instr & 0x000f)] = reg[(instr & 0x000f)] - 1;
            break;

        case 0x4:       //  swap two bytes in a word
            uint16_t temp = (reg[(instr & 0x000f)] & 0xff00) >> 8;
            reg[(instr & 0x000f)] = ((reg[(instr & 0x000f)] & 0x00ff) << 8);
            reg[(instr & 0x000f)] = reg[(instr & 0x000f)] + temp;
            break;

        case 0x5:       //  push the register to stack, decrement stack pointer
            mem[reg[18]] = reg[(instr & 0x000f)];
            reg[18] = reg[18] - 1;
        
        case 0x6:       //  pop from the stack to register
            reg[18] = reg[18] + 1;
            reg[(instr & 0x000f)] = mem[reg[18]];
            break;

        case 0x7:       //  unsigned shift left
            reg[(instr & 0x000f)] = reg[(instr & 0x000f)] << 1;
            if (reg[(instr & 0x000f)] == 0x0){
                reg[21] = reg[21] | 0x4000;
            }
            break;
        
        case 0x8:       //  signed shift left
            uint16_t temp = reg[(instr & 0x000f)] & 0x8000;
            reg[(instr & 0x000f)] = reg[(instr & 0x000f)] << 1;
            if (reg[(instr & 0x000f)] == 0x0){
                reg[21] = reg[21] | 0x4000;
            }
            reg[(instr & 0x000f)] = reg[(instr & 0x000f)] | temp;
            break;

        case 0x9:       //  shift right
            reg[(instr & 0x000f)] = reg[(instr & 0x000f)] >> 1;
            if (reg[(instr & 0x000f)] == 0x0){
                reg[21] = reg[21] | 0x4000;
            }
            break;
        
        case 0xa:   //  signed shift right
            uint16_t temp = reg[(instr & 0x000f)] & 0x8000;
            reg[(instr & 0x000f)] = reg[(instr & 0x000f)] >> 1;
            reg[(instr & 0x000f)] = reg[(instr & 0x000f)] | temp;
            break;

        case 0xb:       //  rotate left
            uint16_t temp = (reg[(instr & 0x000f)] & 0x8000) >> 15;
            reg[(instr & 0x000f)] = reg[(instr & 0x000f)] << 1;
            reg[(instr & 0x000f)] = reg[(instr & 0x000f)] | temp;
            if (reg[(instr & 0x000f)] == 0x0){
                reg[21] = reg[21] | 0x8000;
            }
            break;

        case 0xc:       //  signed rotate left
            uint16_t sign = reg[(instr & 0x000f)] & 0x8000;
            uint16_t temp = (reg[(instr & 0x000f)] & 0x4000) >> 14;
            reg[(instr & 0x000f)] = (((reg[(instr & 0x000f)] & 0x3fff) << 1) | temp) | sign;
            if (reg[(instr & 0x000f)] == 0x0){
                reg[21] = reg[21] | 0x8000;
            }
            if (reg[(instr & 0x000f)] > 0x3fff){
                
            }

    }
    return;
}
void decode4(uint16_t instr){

}
void decode5(uint16_t instr){

}
void cmp(uint16_t instr){

}

