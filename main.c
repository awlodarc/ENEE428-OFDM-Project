#include <stdio.h>
#include "generate_sequence.h"
#include "operations.h"
#include "symbol_modulation.h"
#include "subcarrier_mapping.h"
#include "FFT.h"
#include "IFFT.h"
#include "Add_CP.h"
#include "subcarrier_demapping.h"
#include "channel_estimator.h"

int main() {
    // struct complex fft_freq_samples[FFT_SIZE];
    // struct complex ifft_time_samples[FFT_SIZE];
    // struct complex fft_time_samples[FFT_SIZE];
    // for (int i = 0; i < FFT_SIZE; i++) {
    //     fft_time_samples[i].real = i;
    //     fft_time_samples[i].imag = 0;
    // }
    // struct complex TX_Symbol[FFT_SIZE + CP_LENGTH];
    //
    //
    // //training sub-carriers
    // struct complex training[52] = {{1, 0}, {1, 0}, {-1, 0}, {-1, 0},
    //                             {1, 0}, {1, 0}, {-1, 0}, {1, 0},
    //                             {-1, 0}, {1, 0}, {1, 0}, {1, 0},
    //                             {1, 0}, {1, 0}, {1, 0}, {-1, 0},
    //                             {-1, 0}, {1, 0}, {1, 0}, {-1, 0},
    //                             {1, 0}, {-1, 0}, {1, 0}, {1, 0},
    //                             {1, 0}, {1, 0}, {1, 0}, {-1, 0},
    //                             {-1, 0}, {1, 0}, {1, 0}, {-1, 0},
    //                             {1, 0}, {-1, 0}, {1, 0}, {-1, 0},
    //                             {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0},
    //                             {1, 0}, {1, 0}, {-1, 0}, {-1, 0},
    //                             {1, 0}, {-1, 0}, {1, 0}, {-1, 0},
    //                             {1, 0}, {1, 0}, {1, 0}, {1, 0}
    // };
    //
    //
    // revOrder();
    //
    // twiddleFactorsFFT();
    //
    // fft(fft_freq_samples, fft_time_samples);
    //
    // ifft(ifft_time_samples, fft_freq_samples);
    //
    // for (int p = 0; p <= FFT_SIZE - 1; p++) {
    //     printf("real: %f imag: %f\n", fft_freq_samples[p].real, fft_freq_samples[p].imag);
    // }
    //
    // for (int p = 0; p <= FFT_SIZE - 1; p++) {
    //     printf("real: %f imag: %f\n", ifft_time_samples[p].real, ifft_time_samples[p].imag);
    // }
    //
    // //OFDM generation
    // const unsigned char* seq = get_lfsr_sequence();
    // for (int i = 0; i < out_length; i++) {
    //     //printf("%d, ", seq[i]);
    //
    //     //bpsk modulation api
    //     struct complex* modulated = bpsk(seq);
    //     for (int i = 0; i < 52; i++) {
    //         printf("%f, ", modulated[i].real);
    //     }
    //     printf("\n");
    //
    //     //sub carrier mapping
    //     struct complex* mapped = sub_map(modulated);
    //
    //     for (int i = 0; i < 64; i++) {
    //         printf("index: %d, value: %f, \n", i, mapped[i].real);
    //     }
    //     printf("\n");
    //
    //
    //     //Add CP stuff
    //     Add_CP(TX_Symbol, fft_time_samples);
    //
    //     for (int i = 0; i < FFT_SIZE + CP_LENGTH; i++) {
    //         printf("TX_Symbol[%d] = (%f, %f)\n", i, TX_Symbol[i].real, TX_Symbol[i].imag);
    //     }
    // }
    //
    // struct complex r_true[FFT_SIZE] = {
    //     {1.5, 1.5}, {2.0, 2.0}, {2.5, 2.5}, /* rest 0 or doesn't matter */
    // };
    //
    // bool detection = false;
    //
    // detection = signal_detected(r_true);

    struct complex temp1[52];
    struct complex temp2[52];
    for (int i = 0; i < 52; i++) {
        temp1[i].real = i;
        temp1[i].imag = 0;
        temp2[i].real = i+1;
        temp2[i].imag = 0;
    }

    struct complex H1[52];
    struct complex H2[52];
    struct complex H_est[52];

    for (int i = 0; i < 52; i++) {
        H1[i] = cmultiply(temp1[i], temp1[i]);
        H2[i] = cmultiply(temp2[i], temp2[i]);

        //printf("%f, ", H1[i].real);
        printf("%f, ", H2[i].real);

    }

    estimation(H1, H2, H_est);
    printf("\n\n");

    for (int i = 0; i < 52; i++) {
        printf("%f, ", H_est[i].real);
    }


   return 0;
}


