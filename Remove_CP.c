//
// Created by a_wlo on 4/22/2025.
//

#include "Remove_CP.h"
#include "FFT.h"
#include "Add_CP.h"

void Remove_CP(struct complex* output, struct complex* input) {
    for (int i = 0; i < FFT_SIZE; i++) {
        output[i].real = input[i+CP_LENGTH].real;
        output[i].imag = input[i+CP_LENGTH].imag;
    }
}
