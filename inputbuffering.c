#include "inputbuffering.h"

//  structs
struct termios orig_termios;

void disable_input_buffering() {
    struct termios new_termios;
    tcgetattr(STDIN_FILENO, &orig_termios);
    new_termios = orig_termios;
    new_termios.c_lflag &= ~ICANON;
    new_termios.c_lflag &= ~ECHO;
    new_termios.c_cc[VMIN] = 1;
    new_termios.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

void restore_input_buffering() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

void printScreen(char arr[]){
    for (int i = 0; i < 2000; i++){
        printf("%c", arr[i]);
        if ((i % 80) == 0){
            print("\n");
        }
    }
}