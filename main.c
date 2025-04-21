#include <stdio.h>
#include "generate_sequence.h"
#include "operations.h"
#include "symbol_modulation.h"
#include "FFT.h"
#include "IFFT.h"

int main() {
    struct complex fft_freq_samples[FFT_SIZE];
    struct complex ifft_time_samples[FFT_SIZE];
    struct complex fft_time_samples[FFT_SIZE] = {{1.1016e-02 , 0.0000e+00},
{-9.3180e-01 , 0.0000e+00},
{9.9314e-01 , 0.0000e+00},
{-1.3645e-01 , 0.0000e+00},
{-7.3399e-01 , 0.0000e+00},
{1.4799e+00 , 0.0000e+00},
{7.4623e-01 , 0.0000e+00},
{7.2514e-01 , 0.0000e+00}};
  
  
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

    for (int p = 0; p <= FFT_SIZE - 1; p++) {
        printf("real: %f imag: %f\n", ifft_time_samples[p].real, ifft_time_samples[p].imag);
    }


    //OFDM generation
    const unsigned char* seq = get_lfsr_sequence();
    for (int i = 0; i < out_length; i++) {
        printf("%d, ", seq[i]);
    }
        //bpsk modulation api
        struct complex* modulated = bpsk(seq);

        return 0;
}
