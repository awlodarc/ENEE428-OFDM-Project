//
// Created by eligu on 4/14/2025.
//

#include "operations.h"

struct complex cadd(struct complex x, struct complex y) {
    struct complex z;
    z.real = x.real + y.real;
    z.imag = x.imag + y.imag;
    return z;
}

struct complex csubtract(struct complex x, struct complex y) {
    struct complex z;
    z.real = x.real - y.real;
    z.imag = x.imag - y.imag;
    return z;
}

struct complex cmultiply(struct complex x, struct complex y) {
    struct complex z;
    z.real = x.real * y.real - x.imag * y.imag;
    z.imag = x.real * y.imag + x.imag * y.real;
    return z;
}
