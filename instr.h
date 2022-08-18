#ifndef INSTR_H
#define INSTR_H
#include "computer.h"

//  instruction functions
// nibble 1 = n1, nibble 2 = n2, nibble 3 = n3, nibble 4 = n4
void decode1(uint16_t instr);
void decode2(uint16_t instr);
void decode3(uint16_t instr);
void decode4(uint16_t instr);
void cmp(uint16_t instr);


#endif