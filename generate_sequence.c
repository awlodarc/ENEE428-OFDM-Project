//
// Created by eligu on 4/14/2025.
//

#include "generate_sequence.h"
#include "config.h"
#include <stdio.h>

static unsigned char sequence[out_length];

unsigned int swap_bits(unsigned int num){
    unsigned int reversed = 0;
    int i;
    for(i = 0; i < reg_length; i++){
        //shift over left and fill with lsb of original number
        reversed = (reversed << 1) | (num & 1);
        // shift original number over to the right for new lsb for next loop
        num = num >> 1;
        // each loop creates new lsb in reversed then fills it with original numbers lsb
        // each loop pushes the lsb of num to the left reversing the order of num making the msb the lsb...
    }
    return reversed;
}

const unsigned char* get_lfsr_sequence() {
    unsigned int h = 0b000000000000011;
    const unsigned int init_state = 0b100000000000000;
    unsigned int new_h = swap_bits(h);
    unsigned int state = swap_bits(init_state);
    unsigned int anded;
    int i, m;
    for (i = 0; i < out_length; i++) {
        anded = new_h & state;
        unsigned char sum = 0;
        for (m = 0; m < reg_length; m++) {
            //xor sum and lsb of anded number
            sum = sum ^ (anded & 1);
            //shift anded number right over 1 for next register, makes this the new lsb
            anded = anded >> 1;
        }
        sequence[i] = sum;
        // shift state left over 1 (drops msb) and then fill that spot with sum
        state = (state << 1) | sum;
    }
    return sequence;
}
