//
// Created by a_wlo on 4/28/2025.
//

#include "bit_detection.h"
#include "operations.h"

void bit_detection(struct complex* input, int output[], int mod_type) {

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

            }
        }
    }
}
