
#ifndef _MANDELBROT_FUNCTIONS_H
#define _MANDELBROT_FUNCTIONS_H 1

#include "config.h"

#include <cmath>

typedef void* voidp;
typedef void** voidpp;
typedef unsigned char* ucharp;
typedef double mbnum;

float sqr(const float x);
int point_iteration(const mbnum cx, const mbnum cy, const int depth_max);

#endif
