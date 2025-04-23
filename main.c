#include <stdio.h>
#include "generate_sequence.h"
#include "operations.h"
#include "symbol_modulation.h"
#include "subcarrier_mapping.h"
#include "FFT.h"
#include "IFFT.h"
#include "Add_CP.h"

struct complex ifft_time_samples[FFT_SIZE];
struct complex training_one_time_samples[FFT_SIZE];
struct complex training_two_time_samples[FFT_SIZE];


int main() {
    revOrder();
    twiddleFactorsFFT();


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

    // for (int i = 0; i < 128; i++) {
    //     printf("before[%d] = (%f, %f)\n", i, flat_training[i].real, flat_training[i].imag);
    // }

    //Add CP training
    const int training_length = sizeof(flat_training) / sizeof(flat_training[0]);

    const int num_blocks = training_length / BLOCK_SIZE;
    const int final_train_length = num_blocks * BLOCK_WITH_CP;

    struct complex final_training[final_train_length];

    Add_CP(final_training, flat_training, training_length);

    // for (int i = 0; i < final_train_length; i++) {
    //     printf("TX_Symbol[%d] = (%f, %f)\n", i, final_training[i].real, final_training[i].imag);
    // }


    //OFDM generation
    const unsigned char* seq = get_lfsr_sequence();

    //bpsk modulation api
    struct complex* modulated = bpsk(seq);
    // for (int i = 0; i < 52; i++) {
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

    //sub carrier mapping
    struct complex mapped_symb[num_symb][64] = {0};

    for (int i = 0; i < num_symb; i++) {
        struct complex* mapped = sub_map(symbols[i]);  // Pass 52 elements
        for (int j = 0; j < 64; j++) {
            mapped_symb[i][j] = mapped[j];
        }
    }

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

    //Add CP stuff
    const int symbols_length = sizeof(flattened) / sizeof(flattened[0]);

    const int symbols_num_blocks = symbols_length / BLOCK_SIZE;
    const int final_symbols_length = symbols_num_blocks * BLOCK_WITH_CP;

    struct complex final_symbols[final_symbols_length];

    Add_CP(final_symbols, flattened, final_symbols_length);

    // combine training and OFDM symbols
    struct complex final_TX[final_train_length + final_symbols_length];
    for (int i = 0; i < final_train_length; i++) {
        final_TX[i] = final_training[i];
    }
    for (int i = final_train_length; i < final_symbols_length; i++) {
        final_TX[i] = final_symbols[i];
    }

    for (int i = 0; i < final_train_length + final_symbols_length; i++) {
        printf("real %f, imag %f \n", final_TX[i].real, final_TX[i].imag);
    }
    return 0;
}
