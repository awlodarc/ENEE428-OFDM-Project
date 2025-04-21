//
// Created by eligu on 4/21/2025.
//

#ifndef SYMBOL_MODULATION_H
#define SYMBOL_MODULATION_H

#include "operations.h"

#define bpsk_length 52
#define qpsk_length 104
#define sixteenqam_length 208

// Functions that map input bits to modulation symbols
struct complex* bpsk(const unsigned char* input);
struct complex* qpsk(const unsigned char* input);
struct complex* sixteenqam(const unsigned char* input);

#endif // SYMBOL_MODULATION_H
