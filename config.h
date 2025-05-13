//
// Created by a_wlo on 5/1/2025.
//

#ifndef CONFIG_H
#define CONFIG_H

// Add CP
#define BLOCK_SIZE 64
#define CP_LEN 16
#define BLOCK_WITH_CP (BLOCK_SIZE + CP_LEN)

// FFT
#define pi 3.14159265358979323846
#define FFT_SIZE 64
#define LOG_FFT_SIZE 6

// generate sequence
#define reg_length 15
#define out_length 208

// symbol modulation
#define bpsk_length 52
#define qpsk_length 104
#define sixteenqam_length 208

// modulation type 1 bpsk, 2 qpsk, 4 16qam
#define mod_type 2
#define modulated_length out_length/mod_type

#define CHUNK_SIZE 1024

#endif //CONFIG_H
