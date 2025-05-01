//
// Created by a_wlo on 4/27/2025.
//

#include "channel_estimator.h"
#include "operations.h"

void estimation(struct complex* H1, struct complex* H2, struct complex* H_est) {
  // Call this inside of main when you want to  run it
  // struct complex H1[52];
  // struct complex H2[52];

  //
  // for (int i = 0; i < 52; i++) {
  //   H1[i] = cmultiply(input[i], training_seq[i]);
  //   H2[i] = cmultiply(input[i + 52], training_seq[i]);
  // }

  struct complex temp;
  // Step 2: Average H1 and H2 using cadd
  for (int i = 0; i < 52; i++) {
    temp = cadd(H1[i], H2[i]);

    H_est[i].real = temp.real / 2.0;
    H_est[i].imag = temp.imag / 2.0;
  }
}
