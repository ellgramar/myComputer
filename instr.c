#include "instr.h"

//  instruction functions
void n1(uint16_t instr){
    switch ((instr & 0xf000) >> 12){
        case 0xf:
            n2(instr);
            break;
        case 0x0:
            //  set flags accordingly
            //  if the sum is zero
            if ((((int)(reg[(instr & 0x00f0) >> 4])) + ((int)(reg[instr & 0x000f]))) == 0){
                reg[21] = (reg[21] | 0x4000);
            }
            // if the sum is greater than 65535
            if ((((int)(reg[(instr & 0x00f0) >> 4])) + ((int)(reg[instr & 0x000f]))) > 0xffff){
                reg[21] = (reg[21] | 0x2000);
            }
            // actually add the numbers
            reg[(instr&0x0f00) >> 8] = reg[(instr & 0x00f0) >> 4] + reg[instr & 0x000f];
            break;
    }
}
void n2(uint16_t instr){

}
void n3(uint16_t instr){

}
void n4(uint16_t instr){

}
void cmp(uint16_t instr){

}