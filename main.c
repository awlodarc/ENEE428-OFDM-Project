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


int main() {
    revOrder();
    twiddleFactorsFFT();

    //          Transmitter        //

    //training sub-carriers
    struct complex training[52] = {{1, 0}, {1, 0}, {-1, 0}, {-1, 0}, {1, 0}, {1, 0}, {-1, 0}, {1, 0}, {-1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {-1, 0}, {-1, 0}, {1, 0}, {1, 0}, {-1, 0}, {1, 0}, {-1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {-1, 0}, {-1, 0}, {1, 0}, {1, 0}, {-1, 0}, {1, 0}, {-1, 0}, {1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {1, 0}, {1, 0}, {-1, 0}, {-1, 0}, {1, 0}, {-1, 0}, {1, 0}, {-1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}};

    // S/P for training
    struct complex* mapped_training_one = calloc(64, sizeof(struct complex));
    struct complex* mapped_training_two = calloc(64, sizeof(struct complex));

    sub_map(training, mapped_training_one);
    sub_map(training, mapped_training_two);

    //ifft for training
    struct complex* training_one_time_samples = malloc(FFT_SIZE * sizeof(struct complex));
    struct complex* training_two_time_samples = malloc(FFT_SIZE * sizeof(struct complex));

    ifft(training_one_time_samples, mapped_training_one);
    ifft(training_two_time_samples, mapped_training_two);

    free(mapped_training_one);
    free(mapped_training_two);

    //flatten training
    #define training_length 128
    struct complex* flat_training = malloc(training_length * sizeof(struct complex));
    int i, j;
    for (i = 0; i < 64; i++) {
        flat_training[i] = training_one_time_samples[i];         // Copy first array
        flat_training[i + 64] = training_two_time_samples[i];    // Copy second array, offset by 64
    }

    free(training_one_time_samples);
    free(training_two_time_samples);

    //Add CP training
    #define num_blocks (training_length / BLOCK_SIZE)
    #define final_train_length (num_blocks * BLOCK_WITH_CP)

    struct complex* final_training = malloc(final_train_length * sizeof(struct complex));

    Add_CP(final_training, flat_training, training_length);

    free(flat_training);

    //OFDM generation
    const unsigned char* seq = get_lfsr_sequence();

    for (i = 0; i < 200; i++) {
        printf("%d, ", seq[i]);
    }
    printf("\n\n");

    //modulation api
    struct complex* modulated = malloc(modulated_length * sizeof(struct complex));
    modulate(seq, modulated);

    // for (int i = 0; i < 52; i++) {
    //     printf("%f, %f\n", modulated[i].real, modulated[i].imag);
    // }
    // printf("\n");


    // S/P
    #define sub_size 52
    #define num_symb (modulated_length / sub_size + (modulated_length % sub_size != 0))

    struct complex** symbols = malloc(num_symb * sizeof(struct complex*));
    for (i = 0; i < num_symb; i++) {
        symbols[i] = calloc(sub_size, sizeof(struct complex));
    }

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
    struct complex* mapped = calloc(64, sizeof(struct complex));
    struct complex** mapped_symb = malloc(num_symb * sizeof(struct complex*));
    for (i = 0; i < num_symb; i++) {
        mapped_symb[i] = calloc(64, sizeof(struct complex));
    }

    for (i = 0; i < num_symb; i++) {
        sub_map(symbols[i], mapped);  // Pass 52 elements
        for (j = 0; j < 64; j++) {
            mapped_symb[i][j] = mapped[j];
        }
    }

    for (i = 0; i < num_symb; i++) {
        free(symbols[i]);
    }
    free(symbols);

    // for (int j = 0; j < 64; j++) {
    //     printf("%f, ", mapped_symb[0][j].real);
    // }


    //ifft
    struct complex* ifft_time_samples = malloc(FFT_SIZE * sizeof(struct complex));

    struct complex** ifft_chunks = malloc(num_symb * sizeof(struct complex*));
    for (i = 0; i < num_symb; i++) {
        ifft_chunks[i] = calloc(64, sizeof(struct complex));
    }

    for (i = 0; i < num_symb; i++) {
        ifft(ifft_time_samples, mapped_symb[i]);
        for (j = 0; j < 64; j++) {
            ifft_chunks[i][j] = ifft_time_samples[j];
        }
    }

    for (i = 0; i < num_symb; i++) {
        free(mapped_symb[i]);
    }
    free(mapped_symb);


    // P/S
    #define symbols_length (num_symb * 64)
    struct complex* flattened = malloc(symbols_length * sizeof(struct complex));

    for (i = 0; i < num_symb; i++) {
        for (j = 0; j < 64; j++) {
            flattened[i * 64 + j] = ifft_chunks[i][j];
        }
    }

    for (i = 0; i < num_symb; i++) {
        free(ifft_chunks[i]);
    }
    free(ifft_chunks);

    // for (int j = 368; j < 384; j++) {
    //     printf("%f, ", flattened[j].real);
    // }


    //Add CP stuff
    #define symbols_num_blocks (symbols_length / BLOCK_SIZE)
    #define final_symbols_length (symbols_num_blocks * BLOCK_WITH_CP)

    struct complex* final_symbols = malloc(final_symbols_length * sizeof(struct complex));

    Add_CP(final_symbols, flattened, symbols_length);

    free(flattened);

    // for (int j = 400; j < 480; j++) {
    //     printf("%f, ", final_symbols[j].real);
    // }


    // for (int i = 0; i < final_train_length; i++) {
    //     printf("training final[%d] = (%f, %f)\n", i, final_training[i].real, final_training[i].imag);
    // }

    //combine training and OFDM symbols
    #define final_TX_length (final_train_length + final_symbols_length)
    struct complex* final_TX = malloc(final_TX_length * sizeof(struct complex));

    for (i = 0; i < final_train_length; i++) {
        final_TX[i].real = final_training[i].real;
        final_TX[i].imag = final_training[i].imag;
    }
    for (i = 0; i < final_symbols_length; i++) {
        final_TX[i+final_train_length].real = final_symbols[i].real;
        final_TX[i+final_train_length].imag = final_symbols[i].imag;
    }

    // for (i = 0; i < final_train_length+final_symbols_length; i++) {
    //     printf("index %d: Real %f, Imag %f \n", i, final_TX[i].real, final_TX[i].imag);
    // }



    //          Receiver        //

    //Remove CP
    #define output_len (final_TX_length * BLOCK_SIZE / BLOCK_WITH_CP)

    struct complex* RX_without_CP = malloc(output_len * sizeof(struct complex));
    Remove_CP(RX_without_CP, final_TX, final_TX_length);

    free(final_TX);

    // for (int i = 0; i < output_len; i++) {
    //     printf("real %f, imag %f \n", RX_without_CP[i].real, RX_without_CP[i].imag);
    // }

    //training stuff

    //after timing acquisition we know training is here

    struct complex* Rx_training_one = malloc(64 * sizeof(struct complex));
    struct complex* Rx_training_two = malloc(64 * sizeof(struct complex));

    for (i = 0; i < 64; i++) {
        Rx_training_one[i] = RX_without_CP[i];
        Rx_training_two[i] = RX_without_CP[i+64];
    }

    //fft for RX training
    struct complex* Rx_fft_training_one = malloc(64 * sizeof(struct complex));
    struct complex* Rx_fft_training_two = malloc(64 * sizeof(struct complex));

    fft(Rx_fft_training_one, Rx_training_one);
    fft(Rx_fft_training_two, Rx_training_two);

    free(Rx_training_one);
    free(Rx_training_two);

    //sub-carrier de mapping
    struct complex* Rx_fft_training_one_demapped = malloc(52 * sizeof(struct complex));
    struct complex* Rx_fft_training_two_demapped = malloc(52 * sizeof(struct complex));

    sub_de_map(Rx_fft_training_one, Rx_fft_training_one_demapped);
    sub_de_map(Rx_fft_training_two, Rx_fft_training_two_demapped);

    free(Rx_fft_training_one);
    free(Rx_fft_training_two);

    //channel estimation
    struct complex* H1 = malloc(52 * sizeof(struct complex));
    struct complex* H2 = malloc(52 * sizeof(struct complex));

    for (i = 0; i < 52; i++) {
        H1[i] = cmultiply(Rx_fft_training_one_demapped[i], training[i]);
        H2[i] = cmultiply(Rx_fft_training_two_demapped[i], training[i]);
    }

    free(Rx_fft_training_one_demapped);
    free(Rx_fft_training_two_demapped);

    struct complex* H_est = malloc(52 * sizeof(struct complex));
    estimation(H1, H2, H_est);

    free(H1);
    free(H2);

    //OFDM receiver stuff

    //Only OFDM RX stuff

    #define Rx_data_length (output_len-128)
    struct complex* Rx_data = malloc(Rx_data_length * sizeof(struct complex));

    for (i = 0; i < Rx_data_length; i++) {
        Rx_data[i] = RX_without_CP[i+128];
    }

    free(RX_without_CP);

    // S/P
    #define RX_sub_size 64
    struct complex** RX_symbols = malloc(num_symb * sizeof(struct complex*));
    for (i = 0; i < num_symb; i++) {
        RX_symbols[i] = calloc(RX_sub_size, sizeof(struct complex));
    }

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

    free(Rx_data);

    //OFDM fft
    struct complex* fft_freq_samples = malloc(FFT_SIZE * sizeof(struct complex));

    struct complex** RX_fft_chunks = malloc(num_symb * sizeof(struct complex*));
    for (i = 0; i < num_symb; i++) {
        RX_fft_chunks[i] = calloc(64, sizeof(struct complex));
    }

    for (i = 0; i < num_symb; i++) {
        fft(fft_freq_samples, RX_symbols[i]);
        for (j = 0; j < 64; j++) {
            RX_fft_chunks[i][j] = fft_freq_samples[j];
        }
    }

    for (i = 0; i < num_symb; i++) {
        free(RX_symbols[i]);
    }
    free(RX_symbols);


    //RX sub carrier de-mapper
    struct complex** RX_demapped_symb = malloc(num_symb * sizeof(struct complex*));
    for (i = 0; i < num_symb; i++) {
        RX_demapped_symb[i] = calloc(52, sizeof(struct complex));
    }

    for (i = 0; i < num_symb; i++) {
        struct complex demapped[52];
        sub_de_map(RX_fft_chunks[i], demapped);  // Pass 52 elements
        for (j = 0; j < 52; j++) {
            RX_demapped_symb[i][j] = demapped[j];
        }
    }

    for (i = 0; i < num_symb; i++) {
        free(RX_fft_chunks[i]);
    }
    free(RX_fft_chunks);

    // for (int j = 0; j < 52; j++) {
    //     printf("%f, ", RX_demapped_symb[2][j].real);
    // }
    // printf("\nbreak\n");


    //Equalizer
    struct complex** RX_equalized = malloc(num_symb * sizeof(struct complex*));
    for (i = 0; i < num_symb; i++) {
        RX_equalized[i] = calloc(52, sizeof(struct complex));
    }

    for (i = 0; i < num_symb; i++) {
        struct complex equalized[52];
        equalizer(RX_demapped_symb[i], H_est, equalized);  // Pass 52 elements
        for (j = 0; j < 52; j++) {
            RX_equalized[i][j] = equalized[j];
        }
    }

    free(H_est);

    for (i = 0; i < num_symb; i++) {
        free(RX_demapped_symb[i]);
    }
    free(RX_demapped_symb);

    struct complex* RX_flat = calloc(modulated_length, sizeof(struct complex));

    for (i = 0; i < num_symb; i++) {
        for (j = 0; j < 52; j++) {
            RX_flat[i * 52 + j] = RX_equalized[i][j];
        }
    }

    for (i = 0; i < num_symb; i++) {
        free(RX_equalized[i]);
    }
    free(RX_equalized);


    //bit detector
    #define RX_final_length (modulated_length*mod_type)
    unsigned char* RX_final = malloc(RX_final_length * sizeof(struct complex));
    bit_detection(RX_flat, RX_final);

    free(RX_flat);

    for (i = 0; i < 200; i++) {
        printf("%d, ", RX_final[i]);
    }
    printf("\n\n");


    //bit error rate
    int bit_errors = 0;
    for (i = 0; i < out_length; i++) {
        if (seq[i] != RX_final[i]) {
            bit_errors++;
        }
    }

    double ber = (double)bit_errors / out_length;
    printf("ber %f", ber);

    free(RX_final);

    return 0;
}