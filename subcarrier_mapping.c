//
// Created by eligu on 4/21/2025.
//

#include "subcarrier_mapping.h"

void sub_map(struct complex* input, struct complex* output) {
    int i;
    for (i = 0; i < 26; i++) {
        output[i+38].real = input[i].real;
        output[i+38].imag = input[i].imag;
    }

    for (i = 26; i < 52; i++) {
        output[i-25].real = input[i].real;
        output[i-25].imag = input[i].imag;
    }
}
