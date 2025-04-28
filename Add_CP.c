//
// Created by a_wlo on 4/21/2025.
//

#include "Add_CP.h"
#include "operations.h"
#include "FFT.h"

void Add_CP(struct complex* output, struct complex* input) {
    for (int i = 0; i < CP_LENGTH; i++) {
        output[i] = input[FFT_SIZE - CP_LENGTH + i];
    }

    for (int i = 0; i < FFT_SIZE + CP_LENGTH; i++) {
        output[i + CP_LENGTH] = input[i];
    }

}