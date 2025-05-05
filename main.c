#include <stdio.h>
#include "config.h"
#include <stdlib.h>
#include "generate_sequence.h"
#include "operations.h"
#include "symbol_modulation.h"
#include "subcarrier_mapping.h"
#include "FFT.h"
#include "IFFT.h"
#include "Add_CP.h"
#include "bit_detection.h"
#include "Remove_CP.h"
#include "channel_estimator.h"
#include "equalizer.h"
#include  "Remove_CP.h"
#include "subcarrier_demapping.h"

//          Transmitter        //

//training sub-carriers
struct complex training[52] = {{1, 0}, {1, 0}, {-1, 0}, {-1, 0}, {1, 0}, {1, 0}, {-1, 0}, {1, 0}, {-1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {-1, 0}, {-1, 0}, {1, 0}, {1, 0}, {-1, 0}, {1, 0}, {-1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {-1, 0}, {-1, 0}, {1, 0}, {1, 0}, {-1, 0}, {1, 0}, {-1, 0}, {1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {1, 0}, {1, 0}, {-1, 0}, {-1, 0}, {1, 0}, {-1, 0}, {1, 0}, {-1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}};
struct complex flat_training[128];

#define training_length (sizeof(flat_training) / sizeof(flat_training[0]))
#define num_blocks (training_length / BLOCK_SIZE)
#define final_train_length (num_blocks * BLOCK_WITH_CP)

struct complex final_training[final_train_length];

struct complex modulated[modulated_length];

#define sub_size 52
#define num_symb (modulated_length / sub_size + (modulated_length % sub_size != 0))

struct complex symbols[num_symb][sub_size] = {0};

struct complex mapped_symb[num_symb][64] = {0};

struct complex ifft_time_samples[FFT_SIZE];
struct complex ifft_chunks[num_symb][64] = {0};

struct complex RX_fft_chunks[num_symb][64];

struct complex flattened[num_symb * 64];

#define symbols_length (sizeof(flattened) / sizeof(flattened[0]))
#define symbols_num_blocks (symbols_length / BLOCK_SIZE)
#define final_symbols_length (symbols_num_blocks * BLOCK_WITH_CP)

struct complex final_symbols[final_symbols_length];

struct complex final_TX[final_train_length + final_symbols_length];

//      Receiver     //

#define TX_length (sizeof(final_TX) / sizeof(final_TX[0]))
#define output_len (TX_length * BLOCK_SIZE / BLOCK_WITH_CP)

struct complex RX_without_CP[output_len];

//after timing acquisition we know training is here
struct complex Rx_training_one[64];
struct complex Rx_training_two[64];

struct complex Rx_fft_training_one[64];
struct complex Rx_fft_training_two[64];

struct complex Rx_fft_training_one_demapped[52];
struct complex Rx_fft_training_two_demapped[52];

struct complex H1[52];
struct complex H2[52];

struct complex H_est[52];

#define Rx_data_length (output_len-128)
struct complex Rx_data[Rx_data_length];

#define RX_sub_size 64

struct complex RX_symbols[num_symb][RX_sub_size] = {0};

struct complex fft_freq_samples[FFT_SIZE];

struct complex RX_demapped_symb[num_symb][52];

struct complex RX_equalized[num_symb][52];

unsigned char RX_final[modulated_length*mod_type];

int bit_errors = 0;

int main() {
    revOrder();
    twiddleFactorsFFT();

    //          Transmitter        //
    // S/P for training
    struct complex* mapped_training_one = sub_map(training);
    struct complex* mapped_training_two = sub_map(training);

    //ifft for training
    struct complex training_one_time_samples[FFT_SIZE];
    struct complex training_two_time_samples[FFT_SIZE];

    ifft(training_one_time_samples, mapped_training_one);
    ifft(training_two_time_samples, mapped_training_two);

    //flatten training
    int i, j;
    for (i = 0; i < 64; i++) {
        flat_training[i] = training_one_time_samples[i];         // Copy first array
        flat_training[i + 64] = training_two_time_samples[i];    // Copy second array, offset by 64
    }

    //Add CP training
    Add_CP(final_training, flat_training, training_length);

    //OFDM generation
    const unsigned char* seq = get_lfsr_sequence();

    for (i = 0; i < 200; i++) {
        printf("%d, ", seq[i]);
    }
    printf("\n\n");

    //modulation api
    modulate(seq, modulated);

    // for (int i = 0; i < 52; i++) {
    //     printf("%f, %f\n", modulated[i].real, modulated[i].imag);
    // }
    // printf("\n");


    // S/P
    for (i = 0; i < num_symb; i++) {
        for (j = 0; j < sub_size; j++) {
            int index = i * sub_size + j;
            if (index < modulated_length) {
                symbols[i][j] = modulated[index];
            } else {
                symbols[i][j].real = 0;
                symbols[i][j].imag = 0;
            }
        }
    }


    // for (int j = 0; j < sub_size; j++) {
    //     printf("%f, ", symbols[2][j].real);
    // }
    //
    // printf("\nbreak\n");

    //sub carrier mapping
    for (i = 0; i < num_symb; i++) {
        struct complex* mapped = sub_map(symbols[i]);  // Pass 52 elements
        for (j = 0; j < 64; j++) {
            mapped_symb[i][j] = mapped[j];
        }
    }

    // for (int j = 0; j < 64; j++) {
    //     printf("%f, ", mapped_symb[0][j].real);
    // }

    //ifft
    for (i = 0; i < num_symb; i++) {
        ifft(ifft_time_samples, mapped_symb[i]);
        for (j = 0; j < 64; j++) {
            ifft_chunks[i][j] = ifft_time_samples[j];
        }
    }


    // P/S
    for (i = 0; i < num_symb; i++) {
        for (j = 0; j < 64; j++) {
            flattened[i * 64 + j] = ifft_chunks[i][j];
        }
    }

    // for (int j = 368; j < 384; j++) {
    //     printf("%f, ", flattened[j].real);
    // }


    //Add CP stuff
    Add_CP(final_symbols, flattened, symbols_length);

    // for (int j = 400; j < 480; j++) {
    //     printf("%f, ", final_symbols[j].real);
    // }


    // for (int i = 0; i < final_train_length; i++) {
    //     printf("training final[%d] = (%f, %f)\n", i, final_training[i].real, final_training[i].imag);
    // }

    //combine training and OFDM symbols
    for (i = 0; i < final_train_length; i++) {
        final_TX[i].real = final_training[i].real;
        final_TX[i].imag = final_training[i].imag;
    }
    for (i = 0; i < final_symbols_length; i++) {
        final_TX[i+final_train_length].real = final_symbols[i].real;
        final_TX[i+final_train_length].imag = final_symbols[i].imag;
    }

    // for (int i = 0; i < final_train_length+final_symbols_length; i++) {
    //     printf("index %d: Real %f, Imag %f \n", i, final_TX[i].real, final_TX[i].imag);
    // }

    //          Receiver        //

    //Remove CP
    Remove_CP(RX_without_CP, final_TX, TX_length);

    // for (int i = 0; i < output_len; i++) {
    //     printf("real %f, imag %f \n", RX_without_CP[i].real, RX_without_CP[i].imag);
    // }

    //training stuff
    for (i = 0; i < 64; i++) {
        Rx_training_one[i] = RX_without_CP[i];
        Rx_training_two[i] = RX_without_CP[i+64];
    }

    //fft for RX training
    fft(Rx_fft_training_one, Rx_training_one);
    fft(Rx_fft_training_two, Rx_training_two);

    //sub-carrier de mapping
    sub_de_map(Rx_fft_training_one, Rx_fft_training_one_demapped);
    sub_de_map(Rx_fft_training_two, Rx_fft_training_two_demapped);

    //channel estimation
    for (i = 0; i < 52; i++) {
        H1[i] = cmultiply(Rx_fft_training_one_demapped[i], training[i]);
        H2[i] = cmultiply(Rx_fft_training_two_demapped[i], training[i]);
    }

    estimation(H1, H2, H_est);


    //OFDM receiver stuff

    //Only OFDM RX stuff

    for (i = 0; i < Rx_data_length; i++) {
        Rx_data[i] = RX_without_CP[i+128];
    }

    // S/P
    for (i = 0; i < num_symb; i++) {
        for (j = 0; j < RX_sub_size; j++) {
            int index = i * RX_sub_size + j;
            if (index < Rx_data_length) {
                RX_symbols[i][j] = Rx_data[index];
            } else {
                RX_symbols[i][j].real = 0;
                RX_symbols[i][j].imag = 0;
            }
        }
    }

    //OFDM fft
    for (i = 0; i < num_symb; i++) {
        fft(fft_freq_samples, RX_symbols[i]);
        for (j = 0; j < 64; j++) {
            RX_fft_chunks[i][j] = fft_freq_samples[j];
        }
    }


    //RX sub carrier de-mapper
    for (i = 0; i < num_symb; i++) {
        struct complex demapped[52];
        sub_de_map(RX_fft_chunks[i], demapped);  // Pass 52 elements
        for (j = 0; j < 52; j++) {
            RX_demapped_symb[i][j] = demapped[j];
        }
    }

    // for (int j = 0; j < 52; j++) {
    //     printf("%f, ", RX_demapped_symb[2][j].real);
    // }
    // printf("\nbreak\n");


    //Equalizer

    for (i = 0; i < num_symb; i++) {
        struct complex equalized[52];
        equalizer(RX_demapped_symb[i], H_est, equalized);  // Pass 52 elements
        for (j = 0; j < 52; j++) {
            RX_equalized[i][j] = equalized[j];
        }
    }

    struct complex RX_flat[modulated_length];

    for (i = 0; i < num_symb; i++) {
        for (j = 0; j < 52; j++) {
            RX_flat[i * 52 + j] = RX_equalized[i][j];
        }
    }

    //bit detector
    bit_detection(RX_flat, RX_final);

    for (i = 0; i < 200; i++) {
        printf("%d, ", RX_final[i]);
    }
    printf("\n\n");


    //bit error rate
    for (i = 0; i < out_length; i++) {
        if (seq[i] != RX_final[i]) {
            bit_errors++;
        }
    }

    float ber = (float)bit_errors / out_length;
    printf("ber %f", ber);

    return 0;
}
