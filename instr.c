#include "instr.h"

//  instruction functions
void decode1(uint16_t instr){
    switch ((instr & 0xf000) >> 12){
        case 0xf:       //  escape to decode 2
            decode2(instr);
            break;

        case 0x0:       // addition
            //  set flags accordingly
            //  if the sum is zero
            if ((((int)(reg[(instr & 0x00f0) >> 4])) + ((int)(reg[instr & 0x000f]))) == 0x0){
                reg[20] = (reg[20] | 0x4000);
            }
            // if the sum is greater than 65535
            if ((((int)(reg[(instr & 0x00f0) >> 4])) + ((int)(reg[instr & 0x000f]))) > 0xffff){
                reg[20] = (reg[20] | 0x2000);
            }
            /*   TODO: signed integer overflow flag testing   */
            //  actually add the numbers
            reg[(instr & 0x0f00) >> 8] = reg[(instr & 0x00f0) >> 4] + reg[instr & 0x000f];
            break;

        case 0x1:       //  subtraction
            //  negetive flag test
            if ((((int)(reg[(instr & 0x00f0) >> 4])) - ((int)(reg[instr & 0x000f]))) < 0){
                reg[20] = (reg[20] | 0x8000);
            }
            //  zero-flag test
            if ((((int)(reg[(instr & 0x00f0) >> 4])) - ((int)(reg[instr & 0x000f]))) == 0x0){
                reg[20] = (reg[20] | 0x4000);
            }
            //  negetive flag test
            if (((int)(reg[(instr & 0x00f0) >> 4])) < ((int)(reg[instr & 0x000f]))){
                reg[20] = (reg[20] | 0x2000);
            }
            /*   TODO: signed integer carry flag test   */
            //  actually subtract the numbers
            reg[(instr & 0x0f00) >> 8] = reg[(instr & 0x00f0) >> 4] - reg[instr & 0x000f];
            break;

        case 0x2:       //  multiplication
            //  zero-flag test
            if ((((int)(reg[(instr & 0x00f0) >> 4])) * ((int)(reg[instr & 0x000f]))) == 0x0){
                reg[20] = (reg[20] | 0x4000);
            }
            // if the sum is greater than 65535
            if ((((int)(reg[(instr & 0x00f0) >> 4])) + ((int)(reg[instr & 0x000f]))) > 0xffff){
                reg[20] = (reg[20] | 0x2000);
            }
            /*   TODO: signed integer overflow flag testing   */
            //  actual multiplication
            reg[(instr & 0x0f00) >> 8] = reg[(instr & 0x00f0) >> 4] * reg[instr & 0x000f];
            break;

        case 0x3:       //  integer division
            //  zero flag test
            if (((int)(reg[(instr & 0x00f0) >> 4])) == 0 || ((int)(reg[instr & 0x000f])) == 0x0){
                reg[20] = (reg[20] | 0x4000);
                reg[(instr & 0xf00) >> 8] = 0x0000;
            } else {        //  TODO: signed integer division
                reg[(instr & 0xf00) >> 8] = reg[(instr & 0x00f0) >> 4] / reg[(instr & 0x000f)];
            }
            break;

        case 0x4:      //  remainder finder
            //  zero test
            if ((((int)(reg[(instr & 0x00f0) >> 4])) % ((int)(reg[instr & 0x000f]))) == 0x0){
                reg[20] = (reg[20] | 0x4000);
            }
            reg[(instr & 0x0f00) >> 8] = reg[(instr & 0x00f0) >> 4] % reg[instr & 0x000f];
            break;

        case 0x5:      //  AND
            reg[(instr & 0x0f00) >> 8] = reg[(instr & 0x00f0) >> 4] & reg[(instr & 0x000f)];
            if (reg[(instr & 0x0f00) >> 8] == 0){
                reg[20] = (reg[20] | 0x4000);
            }
            break;

        case 0x6:      //  NAND
            reg[(instr & 0x0f00) >> 8] = ~(reg[(instr & 0x00f0) >> 4] & reg[(instr & 0x000f)]);
            if (reg[(instr & 0x0f00) >> 8] == 0){
                reg[20] = (reg[20] | 0x4000); 
            }
            break;

        case 0x7:      //  OR
            reg[(instr & 0x0f00) >> 8] = reg[(instr & 0x00f0) >> 4] | reg[(instr & 0x000f)];
            if (reg[(instr & 0x0f00) >> 8] == 0){
                reg[20] = (reg[20] | 0x4000);
            }
            break;

        case 0x8:      //  AND
            reg[(instr & 0x0f00) >> 8] = ~(reg[(instr & 0x00f0) >> 4] | reg[(instr & 0x000f)]);
            if (reg[(instr & 0x0f00) >> 8] == 0){
                reg[20] = (reg[20] | 0x4000);
            }
            break;

        case 0x9:      //  XOR
            reg[(instr & 0x0f00) >> 8] = reg[(instr & 0x00f0) >> 4] ^ reg[(instr & 0x000f)];
            if (reg[(instr & 0x0f00) >> 8] == 0){
                reg[20] = (reg[20] | 0x4000);
            }
            break;

        case 0xa:      //  XNOR
            reg[(instr & 0x0f00) >> 8] = ~(reg[(instr & 0x00f0) >> 4] ^ reg[(instr & 0x000f)]);
            if (reg[(instr & 0x0f00) >> 8] == 0){
                reg[20] = (reg[20] | 0x4000);
            }
            break;

        case 0xb:       //  load direct from memory address (lower 256 words, first 4k bits)
            reg[(instr & 0x0f00) >> 8] = mem[(instr & 0x00ff)];
            break;

        case 0xc:       //  store direct from memory address (lower 256 words, first 4k bits)
            mem[(instr & 0x00ff)] = reg[(instr & 0x0f00) >> 8];
            break;

        case 0xd:       //  set r1r1 to the value stored in the lower byte of the instruction
            reg[(instr & 0x0f00) >> 8] = (instr & 0x00ff);
            break;

        default:
            const char Error[] = "Error in decoder1; invalid number";
            break;
    }
    return;
}
void decode2(uint16_t instr){

    if (instr > 0x07ff){
        char c = (char)(reg[15] &0x00ff);
        carr[1][1] = c;
        io = 0x1;
    }
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
        
        case 0x6:       //  store date from r1 to address specified in r2
            mem[reg[instr & 0x000f]] = reg[(instr & 0x00f0) >> 4];
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
                reg[20] = (reg[20] | 0x4000);
            }
            break;

        case 0x2:       //  increment
            if (((int)reg[(instr & 0x000f)]) == 65535){
                reg[20] = reg[20] | 0x6000;
            }
            reg[(instr & 0x000f)] = reg[(instr & 0x000f)] + 1;
            break;

        case 0x3:       //  decrement
            if (((int)reg[(instr & 0x000f)]) == 1){
                reg[20] = reg[20] | 0x4000;
            }
            if (((int)reg[(instr & 0x000f)]) == 0){
                reg[20] = reg[20] | 0x2000;
            }
            reg[(instr & 0x000f)] = reg[(instr & 0x000f)] - 1;
            break;

        case 0x4:       //  swap two bytes in a word
            uint16_t temp = (reg[(instr & 0x000f)] & 0xff00) >> 8;
            reg[(instr & 0x000f)] = ((reg[(instr & 0x000f)] & 0x00ff) << 8);
            reg[(instr & 0x000f)] = reg[(instr & 0x000f)] + temp;
            break;

        case 0x5:       //  push the register to stack, decrement stack pointer
            mem[reg[17]] = reg[(instr & 0x000f)];
            reg[17] = reg[17] - 1;
        
        case 0x6:       //  pop from the stack to register
            reg[17] = reg[17] + 1;
            reg[(instr & 0x000f)] = mem[reg[17]];
            break;

        case 0x7:       //  unsigned shift left
            reg[(instr & 0x000f)] = reg[(instr & 0x000f)] << 1;
            if (reg[(instr & 0x000f)] == 0x0){
                reg[20] = reg[20] | 0x4000;
            }
            break;
        
        case 0x8:       //  signed shift left
            uint16_t temp = reg[(instr & 0x000f)] & 0x8000;
            reg[(instr & 0x000f)] = reg[(instr & 0x000f)] << 1;
            if (reg[(instr & 0x000f)] == 0x0){
                reg[20] = reg[20] | 0x4000;
            }
            reg[(instr & 0x000f)] = reg[(instr & 0x000f)] | temp;
            break;

        case 0x9:       //  shift right
            reg[(instr & 0x000f)] = reg[(instr & 0x000f)] >> 1;
            if (reg[(instr & 0x000f)] == 0x0){
                reg[20] = reg[20] | 0x4000;
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
                reg[20] = reg[20] | 0x8000;
            }
            break;

        case 0xc:       //  signed rotate left
            if (reg[(instr & 0x000f)] > 0x3fff){
                reg[20] = reg[20] | 0x2000;
            }
            uint16_t sign = reg[(instr & 0x000f)] & 0x8000;
            uint16_t temp = (reg[(instr & 0x000f)] & 0x4000) >> 14;
            reg[(instr & 0x000f)] = (((reg[(instr & 0x000f)] & 0x3fff) << 1) | temp) | sign;
            if (reg[(instr & 0x000f)] == 0x0){
                reg[20] = reg[20] | 0x8000;
            }
            if (reg[(instr & 0x000f)] > 0x3fff){
                reg[20] = reg[20] | 0x2000;
            }
            break;
        
        case 0xd:       //  rotate right
            if(reg[(instr & 0x000f)] < 0x2){
                reg[20] = reg[20] | 0x2000;
            }
            uint16_t temp = (reg[(instr & 0x000f)] & 0x0001);
            reg[(instr & 0x000f)] = reg[(instr & 0x000f)] >> 1;
            if (reg[(instr & 0x000f)] == 0x0){
                reg[20] = reg[20] | 0x4000;
            }
            break;
        case 0xe:       //  signed rotate right
            if(reg[(instr & 0x000f)] < 0x2){
                reg[20] = reg[20] | 0x2000;
            }
            uint16_t sign = (reg[(instr & 0x000f)] & 0x8000);
            uint16_t temp = (reg[(instr & 0x000f)] & 0x0001);
            reg[(instr & 0x000f)] = ((reg[(instr & 0x000f)] & 0x3fff) >> 1);
            reg[(instr & 0x000f)] = ((reg[(instr & 0x000f)] | temp) | sign);
            if(reg[(instr & 0x000f)] == 0x0){
                reg[20] = reg[20] | 0x4000;
            }
            break;
        
        case 0xf:       //  escape to decode5
            decode5(instr);
            break;

        default:
            const char Error[] = "Error in decoder3; invalid number";

    }
    return;
}
void decode4(uint16_t instr){
    switch (instr & 0x000f){
        case 0x0:       //  function call
            reg[20] = reg[16];          // store pc in lr for safe keeping
            call();
            break;

        case 0x1:       //  return from a function
            reg[16] = reg[19];      //  set pc to lr, increment sp, set r20 to value of sp
            reg[17] = reg[17] + 1;  //  increment sp
            reg[19] = mem[reg[17]]; //  set lr to the previous push
            break;
        
        case 0x2:       //  no operation
            break;

        case 0x3:       //  push all registers to stack in order
            pushAll();
            break;

        case 0x4:       //  pop all registers from stack
            popAll();
            break;

        case 0x5:       //  push two char values in output register to write buffer in program
            output(reg[14]);
            reg[20] = reg[20] | 0x0400;
            break;

        case 0x6:       //  pull two char values to the input register from read buffer in program
            input(reg[15]);
            reg[20] = reg[20] | 0x0200;
            break;

        case 0x8:       //  clear all flags
            reg[20] = reg[20] & 0x01ff;
            break;

        case 0x9:      //  clear negetive flag
            reg[20] = reg[20] & 0x7fff;
            break;

        case 0xa:       //  clear zero flag
            reg[20] = reg[20] & 0xbfff;
            break;

        case 0xb:       //  clear carry flag
            reg[20] = reg[20] & 0xdfff;
            break;
        
        case 0xc:       //  clear signed overflow flag
            reg[20] = reg[20] & 0xefff;
            break;

        case 0xd:       //  clear jump bits
            reg[20] = reg[20] & 0xfff0;
            break;

        case 0xe:       //  set jump bit (bypass cmp to get into jump mode)
            reg[20] = (reg[20] | 0x0200);

        default:
            const char Error[] = "Error in decoder4; invalid number";
    }
    return;
}
void decode5(uint16_t instr){       //  nothing here yet
    return;
}


// refactor this into a while loop
void cmp(uint16_t instr){
    int upper = (int)(reg[(instr & 0x00f0) >> 4]);
    int lower = (int)(reg[(instr & 0x000f)]);
    setJumpFlags(upper, lower);
    while (reg[20] & 0x0200){       //  stay in jump mode until a branch is taken
        switch ((instr & 0xf000) >> 12){
            case 0x0:       //  absolute jump
                reg[16] = ((instr & 0x0fff) | ((reg[20] & 0x000f) << 12));
                reg[20] = (reg[20] & 0xfdff);       //  0xfdff = 0b1111110111111111 
                break;
            case 0x1:       //  reletive jump positive jump         note overflow
                reg[16] = reg[16] + ((instr & 0x0fff) | (reg[20] & 0x000f) << 12);
                reg[20] = (reg[20] & 0xfdff);       //  clear jump bit
                break;

            case 0x2:       //  reletive jump negetive offset       note underflow
                reg[16] = reg[16] - ((instr & 0x0fff) | (reg[20] & 0x000f) << 12);
                break;
            
            case 0x3:       //  branch if equal
                if (((reg[20] & 0x4000) >> 14) == 0x1){
                    reg[16] = ((instr & 0x0fff) | ((reg[20] & 0x000f) << 12));
                    reg[20] = (reg[20] & 0xfdff);
                }
                break;
            
            case 0x4:       //  branch if not equal
                if (((reg[20] & 0x4000) >> 14) != 0x1){
                    reg[16] = ((instr & 0x0fff) | ((reg[20] & 0x000f) << 12));
                    reg[20] = (reg[20] & 0xfdff);
                }
                break;

            case 0x5:       //  branch if greater than or equal
                if ((((reg[20] & 0x8000) >> 15) == 0x0) || (((reg[20] & 4000) >> 14) == 0x1)){
                    reg[16] = ((instr & 0x0fff) | ((reg[20] & 0x000f) << 12));
                    reg[20] = (reg[20] & 0xfdff);
                }
                break;
            
            case 0x6:       //  branch if greater than
                if (((reg[20] & 0x8000) >> 15) == 0x0){
                    reg[16] = ((instr & 0x0fff) | ((reg[20] & 0x000f) << 12));
                    reg[20] = (reg[20] & 0xfdff);
                }
                break;

            case 0x7:       //  branch if less than or equal 
                if ((((reg[20] & 0x8000) >> 15) == 0x1) || (((reg[20] & 4000) >> 14) == 0x1)){
                    reg[16] = ((instr & 0x0fff) | ((reg[20] & 0x000f) << 12));
                    reg[20] = (reg[20] & 0xfdff);
                }
                break;

            case 0x8:       //  branch if less than 
                if (((reg[20] & 0x8000) >> 15) == 0x1){
                    reg[16] = ((instr & 0x0fff) | ((reg[20] & 0x000f) << 12));
                    reg[20] = (reg[20] & 0xfdff);
                }
                break;
                
            case 0x9:       //  set jump bits (lowest nibble for each) in sr
                reg[20] = ((instr & 0x000f) | (reg[20] & 0xfff0));
                reg[20] = (reg[20] & 0xfdff);
                break;
            
            case 0xa:       //  add to jump bits (lowest nibbles)
                reg[20] = (reg[20] + (instr & 0x000f));
                reg[20] = reg[20] & 0xfe0f;
                break;

            case 0xb:       //  subtract from jump bits
                uint16_t temp = (reg[20] & 0x000f) - (instr & 0x000f);      //  find diff   beware of carries
            //  clear exiesting bits after getting them
                reg[20] = reg[20] | temp;
                break;

            case 0xc:       //  clear jump bits in status register
                reg[20] = (reg[20] & 0xfff0);
                break;
            
            case 0xd:       //  break out of jmp mode
                reg[20] = (reg[20] & 0xfdff);
        }
    }
    return;      
}

void setJumpFlags(int upper, int lower){
    int result = upper - lower;
    if (result < 0){
        reg[20] = (reg[20] | 0x8000);
    }
    if (result == 0){
        reg[20] = (reg[20] | 0x4000);
    }
    if ((upper + lower) > 65535){
        reg[20] = (reg[20] | 0x2000);
    }
    //  TODO: signed integer version
}
void call(){

}
void input(){

}
void output(){
    
}
void pushAll(){
    for (int i = 0; i < 21; i++){
        mem[reg[17]] = reg[i];
        reg[17] = reg[17] - 1;
    }
    reg[18] = reg[17] + 1;
}
void popAll(){
    for (int i = 0; i < 21; i++){
        reg[i] = mem[reg[17]];
        reg[17] = reg[17] + 1;
    }
    return;
}
void chr(){

}