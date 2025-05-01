#include <stdio.h>
#include "config.h"
#include "generate_sequence.h"
#include "operations.h"
#include "symbol_modulation.h"
#include "subcarrier_mapping.h"
#include "FFT.h"
#include "IFFT.h"
#include "Add_CP.h"
#include "Remove_CP.h"

struct complex ifft_time_samples[FFT_SIZE];
struct complex training_one_time_samples[FFT_SIZE];
struct complex training_one_freq_samples[FFT_SIZE];
struct complex training_two_time_samples[FFT_SIZE];

int main() {
    revOrder();
    twiddleFactorsFFT();

    //          Transmitter        //

    //training sub-carriers
    struct complex training[52] = {{1, 0}, {1, 0}, {-1, 0}, {-1, 0}, {1, 0}, {1, 0}, {-1, 0}, {1, 0}, {-1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {-1, 0}, {-1, 0}, {1, 0}, {1, 0}, {-1, 0}, {1, 0}, {-1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {-1, 0}, {-1, 0}, {1, 0}, {1, 0}, {-1, 0}, {1, 0}, {-1, 0}, {1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {1, 0}, {1, 0}, {-1, 0}, {-1, 0}, {1, 0}, {-1, 0}, {1, 0}, {-1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}};

    // S/P for training
    struct complex* mapped_training_one = sub_map(training);
    struct complex* mapped_training_two = sub_map(training);

    //ifft for training
    ifft(training_one_time_samples, mapped_training_one);
    ifft(training_two_time_samples, mapped_training_two);

    //flatten training
    struct complex flat_training[128];
    for (int i = 0; i < 64; i++) {
        flat_training[i] = training_one_time_samples[i];         // Copy first array
        flat_training[i + 64] = training_two_time_samples[i];    // Copy second array, offset by 64
    }

    //Add CP training
    const int training_length = sizeof(flat_training) / sizeof(flat_training[0]);

    const int num_blocks = training_length / BLOCK_SIZE;
    const int final_train_length = num_blocks * BLOCK_WITH_CP;

    struct complex final_training[final_train_length];

    Add_CP(final_training, flat_training, training_length);

    // for (int i = 0; i < final_train_length; i++) {
    //     printf("training final[%d] = (%f, %f)\n", i, final_training[i].real, final_training[i].imag);
    // }

    //OFDM generation
    const unsigned char* seq = get_lfsr_sequence();

    // for (int i = 0; i < 52; i++) {
    //     printf("%d, ", seq[i]);
    // }
    // printf("\n");

    //bpsk modulation api
    struct complex* modulated = bpsk(seq);
    // for (int i = 298; i < 350; i++) {
    //     printf("%f, ", modulated[i].real);
    // }
    // printf("\n");

    // S/P
    #define sub_size 52
    #define num_symb (out_length / sub_size + (out_length % sub_size != 0))

    struct complex symbols[num_symb][sub_size] = {0};

    for (int i = 0; i < num_symb; i++) {
        for (int j = 0; j < sub_size; j++) {
            int index = i * sub_size + j;
            if (index < out_length) {
                symbols[i][j] = modulated[index];
            } else {
                symbols[i][j].real = 0;
                symbols[i][j].imag = 0;
            }
        }
    }


    // for (int j = 36; j < sub_size; j++) {
    //     printf("%f, ", symbols[0][j].real);
    // }


    //sub carrier mapping
    struct complex mapped_symb[num_symb][64] = {0};

    for (int i = 0; i < num_symb; i++) {
        struct complex* mapped = sub_map(symbols[i]);  // Pass 52 elements
        for (int j = 0; j < 64; j++) {
            mapped_symb[i][j] = mapped[j];
        }
    }
    // for (int j = 0; j < 64; j++) {
    //     printf("%f, ", mapped_symb[0][j].real);
    // }


    //ifft
    struct complex ifft_chunks[num_symb][64] = {0};

    for (int i = 0; i < num_symb; i++) {
        ifft(ifft_time_samples, mapped_symb[i]);
        for (int j = 0; j < 64; j++) {
            ifft_chunks[i][j] = ifft_time_samples[j];
        }
    }


    // P/S
    struct complex flattened[num_symb * 64];

    for (int i = 0; i < num_symb; i++) {
        for (int j = 0; j < 64; j++) {
            flattened[i * 64 + j] = ifft_chunks[i][j];
        }
    }

    for (int j = 368; j < 384; j++) {
        printf("%f, ", flattened[j].real);
    }

    printf("\nbreak \n");


    //Add CP stuff
    const int symbols_length = sizeof(flattened) / sizeof(flattened[0]);

    const int symbols_num_blocks = symbols_length / BLOCK_SIZE;
    const int final_symbols_length = symbols_num_blocks * BLOCK_WITH_CP;

    struct complex final_symbols[final_symbols_length];

    Add_CP(final_symbols, flattened, final_symbols_length);

    for (int j = 400; j < 480; j++) {
        printf("%f, ", final_symbols[j].real);
    }

    printf("\nbreak \n");

    //combine training and OFDM symbols
    struct complex final_TX[final_train_length + final_symbols_length];
    for (int i = 0; i < final_train_length; i++) {
        final_TX[i] = final_training[i];
    }
    for (int i = 0; i < final_symbols_length; i++) {
        final_TX[i+final_train_length] = final_symbols[i];
    }

    // for (int i = 0; i < final_train_length + final_symbols_length; i++) {
    //     printf("real %f, imag %f \n", final_TX[i].real, final_TX[i].imag);
    // }

    //          Receiver        //

    //Remove CP
    const int TX_length = sizeof(final_TX) / sizeof(final_TX[0]);

    int output_len = TX_length * BLOCK_SIZE / BLOCK_WITH_CP;
    struct complex RX_without_CP[output_len];
    Remove_CP(RX_without_CP, final_TX, TX_length);

    for (int i = 0; i < output_len; i++) {
        printf("real %f, imag %f \n", RX_without_CP[i].real, RX_without_CP[i].imag);
    }

    return 0;
}
