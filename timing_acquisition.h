//
// Created by a_wlo on 4/26/2025.
//

#ifndef TIMING_ACQUISITION_H
#define TIMING_ACQUISITION_H
#include "operations.h"
#include "stdbool.h"

void correlation(struct complex* r, struct complex* s, int ko, struct complex* convolution);
double corr_avg(struct complex* convolution, int ko);
void corr_max(struct complex* convolution, double* max_value, int* max_index);
bool corr_peak_detection(double* max_value, int* max_index, double c_avg);

#endif //TIMING_ACQUISITION_H
