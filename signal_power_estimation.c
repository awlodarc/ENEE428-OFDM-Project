//
// Created by a_wlo on 4/26/2025.
//

#include "signal_power_estimation.h"
#include "operations.h"
#include "stdbool.h"
#include "config.h"

bool signal_detected(struct complex* r) {
    double p_avg = 0;
    double alpha = 0.5;
    double gamma_p = 1;
    struct complex complex_conj;
    struct complex complex_prod;
    double sq_mag = 0;

    for (int n = 0; n < FFT_SIZE; n++) {
        complex_conj.real = r[n].real;
        complex_conj.imag = -r[n].imag;

        complex_prod = cmultiply(r[n], complex_conj);
        sq_mag = complex_prod.real;

        p_avg = (1 - alpha) * p_avg + alpha * sq_mag;

        if (p_avg > gamma_p) {
            return true;
        }
    }

    return false;
}
