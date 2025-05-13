//
// Created by a_wlo on 4/26/2025.
//

#ifndef TIMING_ACQUISITION_H
#define TIMING_ACQUISITION_H
#include "operations.h"
#include "stdbool.h"

void correlation(struct complex* r, struct complex* s, int k0, struct complex* corr);
void corr_avg(struct complex* corr, float* corr_avg);
void corr_max(struct complex* corr, int k0, float* max_value, int* max_index);
bool corr_peak_detection(double* max_value, int* max_index, double c_avg);

#endif //TIMING_ACQUISITION_H
