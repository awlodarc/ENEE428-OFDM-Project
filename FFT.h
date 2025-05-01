//
// Created by a_wlo on 4/21/2025.
//

#ifndef FFT_H
#define FFT_H
#include "operations.h"

// #define pi 3.14159265358979323846
// #define FFT_SIZE 64
// #define LOG_FFT_SIZE 6

void revOrder();
void twiddleFactorsFFT();
void fft(struct complex* fft_freq_samples, struct complex* fft_time_samples);

#endif //FFT_H
