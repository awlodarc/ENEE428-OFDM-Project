#include <stdio.h>
#include "generate_sequence.h"
#include "operations.h"
#include "symbol_modulation.h"

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

int main() {
    //OFDM generation
    const unsigned char* seq = get_lfsr_sequence();
    for (int i = 0; i < out_length; i++) {
        printf("%d, ", seq[i]);
    }
    printf("\n");

    //bpsk modulation api
    struct complex* modulated = bpsk(seq);
}
