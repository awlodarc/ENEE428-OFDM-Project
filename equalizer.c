//
// Created by a_wlo on 4/28/2025.
//

#include "equalizer.h"
#include "operations.h"

void equalizer(struct complex* input, struct complex* H_est, struct complex* output) {
    for (int i = 0; i < 52; i++) {
        output[i].real = input[i].real / H_est[i].real;
        output[i].imag = input[i].imag / H_est[i].real;
    }
}
