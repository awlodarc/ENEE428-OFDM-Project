//
// Created by eligu on 4/21/2025.
//

#include "subcarrier_mapping.h"

struct complex map_out[64] = {0,0};

struct complex* sub_map(struct complex* input) {
    for (int i = 0; i < 26; i++) {
        map_out[i+38].real = input[i].real;
        map_out[i+38].imag = input[i].imag;
    }

    for (int i = 26; i < 52; i++) {
        map_out[i-25].real = input[i].real;
        map_out[i-25].imag = input[i].imag;
    }

    return map_out;
}
