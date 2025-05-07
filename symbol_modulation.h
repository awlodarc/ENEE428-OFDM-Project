//
// Created by eligu on 4/21/2025.
//

#ifndef SYMBOL_MODULATION_H
#define SYMBOL_MODULATION_H
#include "operations.h"

// #define bpsk_length 52
// #define qpsk_length 104
// #define sixteenqam_length 208

void modulate(const unsigned char* input, struct complex* output);

#endif // SYMBOL_MODULATION_H
