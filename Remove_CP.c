//
// Created by a_wlo on 4/22/2025.
//

#include "Remove_CP.h"
#include "config.h"

void Remove_CP(struct complex* output, struct complex* input, int input_len) {

    int num_blocks = input_len / BLOCK_WITH_CP;

    for (int i = 0; i < num_blocks; i++) {
        int in_start = i * BLOCK_WITH_CP + CP_LEN;
        int out_start = i * BLOCK_SIZE;

        // Copy block without CP
        for (int j = 0; j < BLOCK_SIZE; j++) {
            output[out_start + j] = input[in_start + j];
        }
    }
}