#ifndef INSTR_H
#define INSTR_H
#include "computer.h"

//  instruction functions
void decode1(uint16_t instr);
void decode2(uint16_t instr);
void decode3(uint16_t instr);
void decode4(uint16_t instr);
void decode5(uint16_t instr);
void cmp(uint16_t instr);
void setJumpFlags(int upper, int lower);
void call();
void output();
void input();
void pushAll();
void popAll();

#endif