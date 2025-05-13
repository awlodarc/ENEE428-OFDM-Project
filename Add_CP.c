//
// Created by a_wlo on 4/21/2025.
//

#include "Add_CP.h"
#include "config.h"
#include "operations.h"

void Add_CP(struct complex* output, struct complex* input, int input_len) {

    int num_blocks = input_len / BLOCK_SIZE;

    for (int i = 0; i < num_blocks; i++) {
        int in_start = i * BLOCK_SIZE;
        int cp_start = in_start + BLOCK_SIZE - CP_LEN;
        int out_start = i * BLOCK_WITH_CP;

        // Copy CP
        for (int j = 0; j < CP_LEN; j++) {
            output[out_start + j] = input[cp_start + j];
        }

        // Copy block
        for (int j = 0; j < BLOCK_SIZE; j++) {
            output[out_start + CP_LEN + j] = input[in_start + j];
        }
    }
}