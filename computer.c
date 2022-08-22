#include "computer.h"
#include "inputbuffering.h"
#include "instr.h"

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
    // run the program
    bool running = true;
    //  cycle accumulator
    uint16_t cycles = 0;
    //  set PC to 0x0000
    for (int i = 0; i < 21; i++){
        reg[i] = 0x0000;
    }
    reg[18] = 0xffff;
    //  temporary counter for debugging
    int counter = 0;
    while (running){
        if (counter > 99){
            running = false;
            break;
        }
        // instruction fetch
        uint16_t instr = mem[reg[16]++];
        // decode the instruction 
        // and let the decoder process it
        decode1(instr);

        cycles++;
        if (cycles == 65535){
            cycles = 0;
        }
    }
    // restore input buffering
    restore_input_buffering();
    // run the program
    return 0;
}