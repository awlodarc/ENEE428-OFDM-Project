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
)
