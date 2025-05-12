//
// Created by a_wlo on 4/26/2025.
//

#include "signal_power_estimation.h"
#include "operations.h"
#include "stdbool.h"
#include "config.h"

// bool signal_detected(struct complex* r) {
//     double p_avg = 0;
//     double alpha = 0.5;
//     double gamma_p = 1;
//     struct complex complex_conj;
//     struct complex complex_prod;
//     double sq_mag = 0;
//
//     for (int n = 0; n < FFT_SIZE; n++) {
//         complex_conj.real = r[n].real;
//         complex_conj.imag = -r[n].imag;
//
//         complex_prod = cmultiply(r[n], complex_conj);
//         sq_mag = complex_prod.real;
//
//         p_avg = (1 - alpha) * p_avg + alpha * sq_mag;
//
//         if (p_avg > gamma_p) {
//             return true;
//         }
//     }
//
//     return false;
// }


bool signal_detected(struct complex* r, int* signal_start_index) {
    static double p_avg = 0;
    static int startup_frames = 10;  // Wait for 10 buffers to stabilize
    double alpha = 0.95;
    double gamma_p = 0.0004;
    int n;

    if (startup_frames > 0) {
        startup_frames--;
        return false;  // skip detection during warm-up
    }

    for (n = 0; n < CHUNK_SIZE; n++) {
        double real = r[n].real;
        double imag = r[n].imag;
        double sq_mag = real * real + imag * imag;

        p_avg = (1 - alpha) * p_avg + alpha * sq_mag;

        if (p_avg > gamma_p) {
            *signal_start_index = n;
            return true;
        }
    }

    return false;
}