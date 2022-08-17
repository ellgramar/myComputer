#include "computer.h"

int main(int argc, const char* argv[]){
    // handle no arguments
    if (argc < 2){
        printf("no arguments, what do you think this is?\n");
    }
    // create file pointer
    FILE * fp = fopen(argv[1], "rb");
    // read each uint16_t from the file and store in memory space
    size_t read = fread(mem, sizeof(uint16_t), 16, fp);
    // close file pointer
    fclose(fp);
    // disable input buffering
    disable_input_buffering();
    // restore input buffering
    restore_input_buffering();
    // run the program
    return 0;
}