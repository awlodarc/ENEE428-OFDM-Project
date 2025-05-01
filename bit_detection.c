//
// Created by a_wlo on 4/28/2025.
//

#include "bit_detection.h"
#include "operations.h"

void bit_detection(struct complex* input, int* output, int mod_type) {
    // make sure you have int output[52 * MOD_TYPE]; in main

    if (mod_type == 1) {
        for (int i = 0; i < 52; i++) {
            if (input[i].real > 0) {
                output[i] = 1;
            } else {
                output[i] = 0;
            }
        }
    }

    if (mod_type == 2) {
        for (int i = 0; i < 52; i++) {
            if (input[i].real > 0) {
                output[i] = 1;
            } else if (input[i].real < 0) {
                output[2*i] = 0;
            }

            if (input[i].imag > 0) {
                output[2*i + 1] = 1;
            } else if (input[i].imag < 0) {
                output[2*i + 1] = 0;
            }
        }
    }

    if (mod_type == 4) {
        for (int i = 0; i < 52; i++) {
            if (input[i].real > 2) {
                output[4*i] = 1;
                output[4*i + 1] = 0;
            } else if (input[i].real > 0 && input[i].real < 2) {
                output[4*i] = 1;
                output[4*i + 1] = 1;
            } else if (input[i].real > -2 && input[i].real < 0) {
                output[4*i] = 0;
                output[4*i + 1] = 1;
            } else if (input[i].real < -2) {
                output[4*i] = 0;
                output[4*i + 1] = 0;
            }

            if (input[i].imag > 2) {
                output[4*i + 2] = 1;
                output[4*i + 3] = 0;
            } else if (input[i].imag > 0 && input[i].imag < 2) {
                output[4*i + 2] = 1;
                output[4*i + 3] = 1;
            } else if (input[i].imag > -2 && input[i].imag < 0) {
                output[4*i + 2] = 0;
                output[4*i + 3] = 1;
            } else if (input[i].imag < -2) {
                output[4*i + 2] = 0;
                output[4*i + 3] = 0;
            }

        }
    }
}
