//
// Created by eligu on 4/21/2025.
//

#include "symbol_modulation.h"
#include "operations.h"
#include <math.h>
#include "config.h"

void modulate(const unsigned char* input, struct complex* output){

  //bpsk
  if (mod_type == 1) {
    for (int i = 0; i < out_length; i++) {
      if (input[i] == 0){
        output[i].real = -1;
        output[i].imag = 0;
      }
      else if (input[i] == 1){
        output[i].real = 1;
        output[i].imag = 0;
      }
    }
  }

//qpsk
  if (mod_type == 2) {
    for (int i = 0; i < out_length/2; i++) {
      int bit1 = input[2*i];
      int bit2 = input[2*i + 1];

      // Form the bit pair
      if (bit1 == 0 && bit2 == 0) {
        output[i].real = -1.0 / sqrt(2);
        output[i].imag = -1.0 / sqrt(2);
      } else if (bit1 == 0 && bit2 == 1) {
        output[i].real = -1.0 / sqrt(2);
        output[i].imag = 1.0 / sqrt(2);
      } else if (bit1 == 1 && bit2 == 1) {
        output[i].real = 1.0 / sqrt(2);
        output[i].imag = 1.0 / sqrt(2);
      } else if (bit1 == 1 && bit2 == 0) {
        output[i].real = 1.0 / sqrt(2);
        output[i].imag = -1.0 / sqrt(2);
      }
    }
  }

  //16qam
  if (mod_type == 4) {
    for (int i = 0; i < out_length/4; i++) {
      int bit4 = input[4*i];
      int bit3 = input[4*i + 1];
      int bit2 = input[4*i + 2];
      int bit1 = input[4*i + 3];

      // Form the bit pair
      if (bit1 == 0 && bit2 == 0 && bit3 == 1 && bit4 == 0) {
        output[i].real = -3.0 / sqrt(10);
        output[i].imag = 3.0 / sqrt(10);
      } else if (bit1 == 0 && bit2 == 1 && bit3 == 1 && bit4 == 0) {
        output[i].real = -1.0 / sqrt(10);
        output[i].imag = 3.0 / sqrt(10);
      } else if (bit1 == 1 && bit2 == 1 && bit3 == 1 && bit4 == 0) {
        output[i].real = 1.0 / sqrt(10);
        output[i].imag = 3.0 / sqrt(10);
      } else if (bit1 == 1 && bit2 == 0 && bit3 == 1 && bit4 == 0) {
        output[i].real = 3.0 / sqrt(10);
        output[i].imag = 3.0 / sqrt(10);
      } else if (bit1 == 0 && bit2 == 0 && bit3 == 1 && bit4 == 1) {
        output[i].real = -3.0 / sqrt(10);
        output[i].imag = 1.0 / sqrt(10);
      } else if (bit1 == 0 && bit2 == 1 && bit3 == 1 && bit4 == 1) {
        output[i].real = -1.0 / sqrt(10);
        output[i].imag = 1.0 / sqrt(10);
      } else if (bit1 == 1 && bit2 == 1 && bit3 == 1 && bit4 == 1) {
        output[i].real = 1.0 / sqrt(10);
        output[i].imag = 1.0 / sqrt(10);
      } else if (bit1 == 1 && bit2 == 0 && bit3 == 1 && bit4 == 1) {
        output[i].real = 3.0 / sqrt(10);
        output[i].imag = 1.0 / sqrt(10);
      } else if (bit1 == 0 && bit2 == 0 && bit3 == 0 && bit4 == 1) {
        output[i].real = -3.0 / sqrt(10);
        output[i].imag = -1.0 / sqrt(10);
      } else if (bit1 == 0 && bit2 == 1 && bit3 == 0 && bit4 == 1) {
        output[i].real = -1.0 / sqrt(10);
        output[i].imag = -1.0 / sqrt(10);
      } else if (bit1 == 1 && bit2 == 1 && bit3 == 0 && bit4 == 1) {
        output[i].real = 1.0 / sqrt(10);
        output[i].imag = -1.0 / sqrt(10);
      } else if (bit1 == 1 && bit2 == 0 && bit3 == 0 && bit4 == 1) {
        output[i].real = 3.0 / sqrt(10);
        output[i].imag = -1.0 / sqrt(10);
      } else if (bit1 == 0 && bit2 == 0 && bit3 == 0 && bit4 == 0) {
        output[i].real = -3.0 / sqrt(10);
        output[i].imag = -3.0 / sqrt(10);
      } else if (bit1 == 0 && bit2 == 1 && bit3 == 0 && bit4 == 0) {
        output[i].real = -1.0 / sqrt(10);
        output[i].imag = -3.0 / sqrt(10);
      } else if (bit1 == 1 && bit2 == 1 && bit3 == 0 && bit4 == 0) {
        output[i].real = 1.0 / sqrt(10);
        output[i].imag = -3.0 / sqrt(10);
      } else if (bit1 == 1 && bit2 == 0 && bit3 == 0 && bit4 == 0) {
        output[i].real = 3.0 / sqrt(10);
        output[i].imag = -3.0 / sqrt(10);
      }
    }
  }
}
