#include "instr.h"

//  instruction functions
void decode1(uint16_t instr){
    switch ((instr & 0xf000) >> 12){
        case 0xf:       //  cmp
            n2(instr);
            break;

        case 0x0:       // addition
            //  set flags accordingly
            //  if the sum is zero
            if ((((int)(reg[(instr & 0x00f0) >> 4])) + ((int)(reg[instr & 0x000f]))) == 0){
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
            if ((((int)(reg[(instr & 0x00f0) >> 4])) - ((int)(reg[instr & 0x000f]))) == 0){
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
            if ((((int)(reg[(instr & 0x00f0) >> 4])) * ((int)(reg[instr & 0x000f]))) == 0){
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

    }
}
void decode2(uint16_t instr){

}
void decode3(uint16_t instr){

}
void decode4(uint16_t instr){

}
void cmp(uint16_t instr){

}

