//
// Created by a_wlo on 4/26/2025.
//

#include "timing_acquisition.h"
#include "operations.h"
#include "stdbool.h"
#include "config.h"

void correlation(struct complex* r, struct complex* s, int k0, struct complex* corr) {
    struct complex complex_conj;
    struct complex product;
    struct complex sum;

    for (int k = 0; k < FFT_SIZE; k++) {
        sum.real = 0;
        sum.imag = 0;

        for (int n = 0; n < FFT_SIZE; n++) {
            complex_conj.real = r[n + k0 + k].real;
            complex_conj.imag = -r[n + k0 + k].imag;

            product = cmultiply(s[n], complex_conj);
            sum.real += product.real;
            sum.imag += product.imag;
        }

        corr[k] = sum;  // Output goes into corr[0..FFT_SIZE-1]
    }
}

void corr_avg(struct complex* corr, float* avg_corr) {
    float sq_mag = 0;
    *avg_corr = 0;

    for (int k = 0; k < FFT_SIZE; k++) {
        sq_mag = corr[k].real * corr[k].real + corr[k].imag * corr[k].imag;
        *avg_corr += sq_mag / FFT_SIZE;
    }
}

void corr_max(struct complex* corr, int k0, float* max_value, int* max_index) {
    float sq_mag = 0;

    for (int k = 0; k < FFT_SIZE; k++) {
        sq_mag = corr[k].real * corr[k].real + corr[k].imag * corr[k].imag;

        if (sq_mag > *max_value) {
            *max_value = sq_mag;
            *max_index = k+k0;
        }
    }
}

bool corr_peak_detection(double* max_value, int* max_index, double c_avg) {
    double gamma_c = 1;
    double ratio = (*max_value) / c_avg;

    if (ratio > gamma_c) {
        return true;
    }

    return false;
}


