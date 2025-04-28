//
// Created by a_wlo on 4/28/2025.
//

#include "subcarrier_demapping.h"
#include "operations.h"

void sub_de_map(struct complex* fft_out, struct complex* output) {
    for (int i = 0; i < 26; i++) {
        output[i].real = fft_out[i + 38].real;
        output[i].imag = fft_out[i + 38].imag;
    }

    for (int i = 0; i < 26; i++) {
        output[i + 26].real = fft_out[i + 1].real;
        output[i + 26].imag = fft_out[i + 1].imag;
    }
}
