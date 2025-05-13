//
// Created by a_wlo on 4/21/2025.
//

#include "IFFT.h"
#include "FFT.h"
#include "operations.h"
#include "config.h"

struct complex temp_in[FFT_SIZE];
struct complex temp_out[FFT_SIZE];
struct complex output[FFT_SIZE];

void ifft(struct complex* output, struct complex* input) {
    for (int i = 0; i < FFT_SIZE; i++) {
        temp_in[i].real = input[i].real;
        temp_in[i].imag = -input[i].imag;
    }

    fft(temp_out, temp_in);

    for (int i = 0; i < FFT_SIZE; i++) {
        output[i].real = temp_out[i].real / FFT_SIZE;
        output[i].imag = -temp_out[i].imag / FFT_SIZE;
    }
}