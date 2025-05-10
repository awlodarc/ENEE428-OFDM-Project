//
// Created by a_wlo on 4/28/2025.
//

#include "bit_detection.h"
#include "operations.h"
#include "math.h"
#include "config.h"

void bit_detection(struct complex* input, unsigned char* output) {
    // make sure you have int output[52 * MOD_TYPE]; in main

    if (mod_type == 1) {
        for (int i = 0; i < modulated_length; i++) {
            if (input[i].real > 0) {
                output[i] = 1;
            } else {
                output[i] = 0;
            }
        }
    }

    if (mod_type == 2) {
        for (int i = 0; i < modulated_length; i++) {
            float re = input[i].real*sqrt(2);
            float im = input[i].imag*sqrt(2);

            if (re < 0 && im < 0) {
                output[2 * i]     = 0;
                output[2 * i + 1] = 0;
            } else if (re < 0 && im > 0) {
                output[2 * i]     = 0;
                output[2 * i + 1] = 1;
            } else if (re > 0 && im > 0) {
                output[2 * i]     = 1;
                output[2 * i + 1] = 1;
            } else if (re > 0 && im < 0) {
                output[2 * i]     = 1;
                output[2 * i + 1] = 0;
            }
        }
    }

    if (mod_type == 4) {
        for (int i = 0; i < modulated_length; i++) {
            float re = input[i].real*sqrt(10);
            float im = input[i].imag*sqrt(10);

            if (re < -2) {
                output[4*i + 3] = 0;
                output[4*i + 2] = 0;
            } else if (-2 <= re && re < 0) {
                output[4*i + 3] = 0;
                output[4*i + 2] = 1;
            } else if (0 <= re && re < 2) {
                output[4*i + 3] = 1;
                output[4*i + 2] = 1;
            } else if ( re >= 2) {
                output[4*i + 3] = 1;
                output[4*i + 2] = 0;
            }

            if (im < -2) {
                output[4*i + 1] = 0;
                output[4*i + 0] = 0;
            } else if (-2 <= im &&  im < 0) {
                output[4*i + 1] = 0;
                output[4*i + 0] = 1;
            } else if (0 <= im && im < 2) {
                output[4*i + 1] = 1;
                output[4*i + 0] = 1;
            } else if ( im >= 2) {
                output[4*i + 1] = 1;
                output[4*i + 0] = 0;
            }

        }
    }
}
