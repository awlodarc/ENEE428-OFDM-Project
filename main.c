#include <stdio.h>
#include "generate_sequence.h"
#include "operations.h"
#include "symbol_modulation.h"
#include "FFT.h"
#include "IFFT.h"
#include "Add_CP.h"

int main() {
    struct complex fft_freq_samples[FFT_SIZE];
    struct complex ifft_time_samples[FFT_SIZE];
    struct complex fft_time_samples[FFT_SIZE];
    for (int i = 0; i < FFT_SIZE; i++) {
        fft_time_samples[i].real = i;
        fft_time_samples[i].imag = 0;
    }
    struct complex TX_Symbol[FFT_SIZE + CP_LENGTH];
  
  
    //training sub-carriers
    struct complex training[52] = {{1, 0}, {1, 0}, {-1, 0}, {-1, 0},
                                {1, 0}, {1, 0}, {-1, 0}, {1, 0},
                                {-1, 0}, {1, 0}, {1, 0}, {1, 0},
                                {1, 0}, {1, 0}, {1, 0}, {-1, 0},
                                {-1, 0}, {1, 0}, {1, 0}, {-1, 0},
                                {1, 0}, {-1, 0}, {1, 0}, {1, 0},
                                {1, 0}, {1, 0}, {1, 0}, {-1, 0},
                                {-1, 0}, {1, 0}, {1, 0}, {-1, 0},
                                {1, 0}, {-1, 0}, {1, 0}, {-1, 0},
                                {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0},
                                {1, 0}, {1, 0}, {-1, 0}, {-1, 0},
                                {1, 0}, {-1, 0}, {1, 0}, {-1, 0},
                                {1, 0}, {1, 0}, {1, 0}, {1, 0}
    };


    revOrder();

    twiddleFactorsFFT();

    fft(fft_freq_samples, fft_time_samples);

    ifft(ifft_time_samples, fft_freq_samples);

    // for (int p = 0; p <= FFT_SIZE - 1; p++) {
    //     printf("real: %f imag: %f\n", fft_freq_samples[p].real, fft_freq_samples[p].imag);
    // }

    // for (int p = 0; p <= FFT_SIZE - 1; p++) {
    //     printf("real: %f imag: %f\n", ifft_time_samples[p].real, ifft_time_samples[p].imag);
    // }


    //OFDM generation
    const unsigned char* seq = get_lfsr_sequence();
    for (int i = 0; i < out_length; i++) {
        //printf("%d, ", seq[i]);
    }
        //bpsk modulation api
        struct complex* modulated = bpsk(seq);

    Add_CP(TX_Symbol, fft_time_samples);

    for (int i = 0; i < FFT_SIZE + CP_LENGTH; i++) {
        printf("TX_Symbol[%d] = (%f, %f)\n", i, TX_Symbol[i].real, TX_Symbol[i].imag);
    }

        return 0;
}
