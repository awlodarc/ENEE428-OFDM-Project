//
// Created by a_wlo on 4/21/2025.
//

#include "FFT.h"
#include "operations.h"
#include "config.h"
#include <math.h>
#include <stdlib.h>

struct complex Wfft[FFT_SIZE/2];
int bitrev[FFT_SIZE];

void revOrder(){
    for (int i = 0; i <= FFT_SIZE-1; i++){
        bitrev[i] = i;
        unsigned int NO_OF_BITS = LOG_FFT_SIZE;
        unsigned int reverse_num = 0;

        for(int k = 0; k< NO_OF_BITS; k++){
            if((bitrev[i]&(1<<k)))
                reverse_num |= 1<<((NO_OF_BITS-1) -k);
        }
        bitrev[i] = reverse_num;
    }
}

void twiddleFactorsFFT(){
    struct complex root;

    for (int k = 0; k <= FFT_SIZE/2 - 1; k++){
        root.real = cos((2*pi)*k/(FFT_SIZE));
        root.imag = -sin((2*pi)*k/(FFT_SIZE));

        Wfft[k].real = root.real;
        Wfft[k].imag = root.imag;
    }
}

void fft(struct complex* fft_freq_samples, struct complex* fft_time_samples){
    int g, s, b, r;

    struct complex product;
    struct complex* first = calloc(FFT_SIZE, sizeof(struct complex));
    //struct complex first[FFT_SIZE];
    //struct complex second[FFT_SIZE];
    struct complex* second = calloc(FFT_SIZE, sizeof(struct complex));

    struct complex *current = first;
    struct complex *next = second;

    int i1, i2;

    for(r = 0; r <= FFT_SIZE-1; r++){
        //printf("%d \n", bitrev[r]);
        //printf("%f, %f\n", fft_time_samples[r].real, fft_time_samples[r].imag);
        current[r] = fft_time_samples[bitrev[r]];
        //printf("%f, %f\n", current[r].real, current[r].imag);
    }

    for(s = 1; s <= LOG_FFT_SIZE; s++){

        int num_groups = FFT_SIZE/(1<<s);
        int group_size = (1<<(s-1));
        int group_offset = FFT_SIZE/num_groups;

        for(g = 0; g <= num_groups - 1; g++){
            for(b = 0; b <= group_size-1; b++){

                i1 = group_offset * g + b;
                i2 = i1 + group_size;
                product = cmultiply(current[i2], Wfft[(num_groups*b)]);
                next[i1] = cadd(current[i1], product);
                next[i2] = csubtract(current[i1], product);
            }
        }

        struct complex* temp = next;
        next = current;
        current = temp;
    }

    for(r = 0; r <= FFT_SIZE-1; r++){
        fft_freq_samples[r] = current[r];
    }
    free(first);
    free(second);
}
