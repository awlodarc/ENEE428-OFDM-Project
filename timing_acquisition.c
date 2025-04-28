//
// Created by a_wlo on 4/26/2025.
//

#include "timing_acquisition.h"
#include "operations.h"
#include "FFT.h"
#include "stdbool.h"

void correlation(struct complex* r, struct complex* s, int ko, struct complex* convolution) {
    struct complex complex_conj;
    struct complex product;
    struct complex sum;

    for (int k = ko; k < ko + FFT_SIZE; k++) {
        sum.real = 0;
        sum.imag = 0;

        for (int n = 0; n < FFT_SIZE; n++) {
            complex_conj.real = r[n+k].real;
            complex_conj.imag = -r[n+k].imag;

            product = cmultiply(s[n], complex_conj);

            sum.real += product.real;
            sum.imag += product.imag;
        }

        convolution[k] = sum;
    }
}

double corr_avg(struct complex* convolution, int ko) {
    double c_avg = 0;
    struct complex complex_conj;
    struct complex complex_prod;
    double sq_mag = 0;

    for (int k = ko; k < ko + FFT_SIZE; k++) {
        complex_conj.real = convolution[k].real;
        complex_conj.imag = -convolution[k].imag;

        complex_prod = cmultiply(convolution[k], complex_conj);
        sq_mag = complex_prod.real;

        c_avg += sq_mag / FFT_SIZE;
    }

    return c_avg;
}

void corr_max(struct complex* convolution, double* max_value, int* max_index) {
    *max_value = 0;
    *max_index = 0;
    struct complex complex_conj;
    struct complex complex_prod;
    double sq_mag = 0;

    for (int k = 0; k < FFT_SIZE; k++) {
        complex_conj.real = convolution[k].real;
        complex_conj.imag = -convolution[k].imag;

        complex_prod = cmultiply(convolution[k], complex_conj);
        sq_mag = complex_prod.real;

        if (sq_mag > *max_value) {
            *max_value = sq_mag;
            *max_index = k;
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


