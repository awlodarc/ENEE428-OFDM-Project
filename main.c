#include <stdio.h>
#include "generate_sequence.h"
#include "operations.h"

int main() {
    const unsigned char* seq = get_lfsr_sequence();
    for (int i = 0; i < out_length; i++) {
        printf("%d, ", seq[i]);
    }
    printf("\n");

    printf("Test Andrew branch");

    return 0;
}
