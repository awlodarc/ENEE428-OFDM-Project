cmake_minimum_required(VERSION 3.30)
project(Final_Project C)

set(CMAKE_C_STANDARD 11)

add_executable(Final_Project main.c
        generate_sequence.c
        generate_sequence.h
        operations.c
        operations.h
        IFFT.c
        IFFT.h
        FFT.c
        FFT.h
        symbol_modulation.c
        symbol_modulation.h
        Add_CP.c
        Add_CP.h
        subcarrier_mapping.c
        subcarrier_mapping.h
        signal_power_estimation.c
        signal_power_estimation.h
        timing_acquisition.c
        timing_acquisition.h
        channel_estimator.c
        channel_estimator.h
        equalizer.c
        equalizer.h
        bit_detection.c
        bit_detection.h
        subcarrier_demapping.c
        subcarrier_demapping.h
        Remove_CP.c
        Remove_CP.h
        config.c
        config.h
)
