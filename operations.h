//
// Created by eligu on 4/14/2025.
//

#ifndef OPERATIONS_H
#define OPERATIONS_H

// Complex number structure
struct complex{
    float real;
    float imag;
};

// Complex operations (returning struct by value)
struct complex cadd(struct complex x, struct complex y);
struct complex csubtract(struct complex x, struct complex y);
struct complex cmultiply(struct complex x, struct complex y);

#endif // OPERATIONS_H
