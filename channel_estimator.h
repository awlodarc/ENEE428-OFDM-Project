//
// Created by a_wlo on 4/27/2025.
//

#ifndef CHANNEL_ESTIMATOR_H
#define CHANNEL_ESTIMATOR_H
#include "operations.h"

void estimation(struct complex* input, struct complex* training_seq, struct complex* H_est);

#endif //CHANNEL_ESTIMATOR_H
