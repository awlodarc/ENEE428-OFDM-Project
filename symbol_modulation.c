//
// Created by eligu on 4/21/2025.
//

#include "symbol_modulation.h"
#include "operations.h"
#include <math.h>
#include "config.h"

//bpsk
struct complex bpsk_out[out_length];

struct complex* bpsk(const unsigned char* input) {
    for (int i = 0; i < out_length; i++) {
      if (input[i] == 0){
        bpsk_out[i].real = -1;
        bpsk_out[i].imag = 0;
      }
      else if (input[i] == 1){
        bpsk_out[i].real = 1;
        bpsk_out[i].imag = 0;
      }
    }
    return bpsk_out;
}

//qpsk

struct complex qpsk_out[out_length/2];

struct complex* qpsk(const unsigned char* input) {
  for (int i = 0; i < out_length/2; i++) {
    int bit1 = input[2*i];
    int bit2 = input[2*i + 1];

    // Form the bit pair
    if (bit1 == 0 && bit2 == 0) {
      qpsk_out[i].real = -1.0 / sqrt(2);
      qpsk_out[i].imag = -1.0 / sqrt(2);
    } else if (bit1 == 0 && bit2 == 1) {
      qpsk_out[i].real = -1.0 / sqrt(2);
      qpsk_out[i].imag = 1.0 / sqrt(2);
    } else if (bit1 == 1 && bit2 == 1) {
      qpsk_out[i].real = 1.0 / sqrt(2);
      qpsk_out[i].imag = 1.0 / sqrt(2);
    } else if (bit1 == 1 && bit2 == 0) {
      qpsk_out[i].real = 1.0 / sqrt(2);
      qpsk_out[i].imag = -1.0 / sqrt(2);
    }
  }
  return qpsk_out;
}

struct complex sixteenqam_out[out_length/4];

struct complex* sixteenqam(const unsigned char* input) {
  for (int i = 0; i < out_length/4; i++) {
    int bit1 = input[4*i];
    int bit2 = input[4*i + 1];
    int bit3 = input[4*i + 2];
    int bit4 = input[4*i + 3];

    // Form the bit pair
    if (bit1 == 0 && bit2 == 0 && bit3 == 1 && bit4 == 0) {
      sixteenqam_out[i].real = -3.0 / sqrt(10);
      sixteenqam_out[i].imag = 3.0 / sqrt(10);
    } else if (bit1 == 0 && bit2 == 1 && bit3 == 1 && bit4 == 0) {
      sixteenqam_out[i].real = -1.0 / sqrt(10);
      sixteenqam_out[i].imag = 3.0 / sqrt(10);
    } else if (bit1 == 1 && bit2 == 1 && bit3 == 1 && bit4 == 0) {
      sixteenqam_out[i].real = 1.0 / sqrt(10);
      sixteenqam_out[i].imag = 3.0 / sqrt(10);
    } else if (bit1 == 1 && bit2 == 0 && bit3 == 1 && bit4 == 0) {
      sixteenqam_out[i].real = 3.0 / sqrt(10);
      sixteenqam_out[i].imag = 3.0 / sqrt(10);
    } else if (bit1 == 0 && bit2 == 0 && bit3 == 1 && bit4 == 1) {
      sixteenqam_out[i].real = -3.0 / sqrt(10);
      sixteenqam_out[i].imag = 1.0 / sqrt(10);
    } else if (bit1 == 0 && bit2 == 1 && bit3 == 1 && bit4 == 1) {
      sixteenqam_out[i].real = -1.0 / sqrt(10);
      sixteenqam_out[i].imag = 1.0 / sqrt(10);
    } else if (bit1 == 1 && bit2 == 1 && bit3 == 1 && bit4 == 1) {
      sixteenqam_out[i].real = 1.0 / sqrt(10);
      sixteenqam_out[i].imag = 1.0 / sqrt(10);
    } else if (bit1 == 1 && bit2 == 0 && bit3 == 1 && bit4 == 1) {
      sixteenqam_out[i].real = 3.0 / sqrt(10);
      sixteenqam_out[i].imag = -1.0 / sqrt(10);
    } else if (bit1 == 0 && bit2 == 0 && bit3 == 0 && bit4 == 1) {
      sixteenqam_out[i].real = -3.0 / sqrt(10);
      sixteenqam_out[i].imag = -1.0 / sqrt(10);
    } else if (bit1 == 0 && bit2 == 1 && bit3 == 0 && bit4 == 1) {
      sixteenqam_out[i].real = -1.0 / sqrt(10);
      sixteenqam_out[i].imag = -1.0 / sqrt(10);
    } else if (bit1 == 1 && bit2 == 1 && bit3 == 0 && bit4 == 1) {
      sixteenqam_out[i].real = 1.0 / sqrt(10);
      sixteenqam_out[i].imag = -1.0 / sqrt(10);
    } else if (bit1 == 1 && bit2 == 0 && bit3 == 0 && bit4 == 1) {
      sixteenqam_out[i].real = 3.0 / sqrt(10);
      sixteenqam_out[i].imag = -1.0 / sqrt(10);
    } else if (bit1 == 0 && bit2 == 0 && bit3 == 0 && bit4 == 0) {
      sixteenqam_out[i].real = -3.0 / sqrt(10);
      sixteenqam_out[i].imag = -3.0 / sqrt(10);
    } else if (bit1 == 0 && bit2 == 1 && bit3 == 0 && bit4 == 0) {
      sixteenqam_out[i].real = -1.0 / sqrt(10);
      sixteenqam_out[i].imag = -3.0 / sqrt(10);
    } else if (bit1 == 1 && bit2 == 1 && bit3 == 0 && bit4 == 0) {
      sixteenqam_out[i].real = 1.0 / sqrt(10);
      sixteenqam_out[i].imag = -3.0 / sqrt(10);
    } else if (bit1 == 1 && bit2 == 0 && bit3 == 0 && bit4 == 0) {
      sixteenqam_out[i].real = 3.0 / sqrt(10);
      sixteenqam_out[i].imag = -3.0 / sqrt(10);
    }
  }
  return sixteenqam_out;
}
