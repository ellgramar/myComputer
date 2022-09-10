#ifndef INPUTBUFFERING_H
#define INPUTBUFFERING_H
//  includes
#include <sys/termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
//  functions
void disable_input_buffering();
void restore_input_buffering();
#endif