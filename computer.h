#ifndef MYCOMPUTER_H
#define MYCOMPUTER_H
//  Includes
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
//  needed for i/o support
#include <sys/termios.h>
#include <unistd.h>

//  register file
uint16_t reg[21];
//  memory
uint16_t mem[0xffff];
//  error handling
extern char Error[64];

#endif