
#ifndef _MANDELBROT_H
#define _MANDELBROT_H 1

#include "config.h"

#include <iostream>
#include <cmath>
#include <complex>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
//#include <math.h>
#include <time.h>

#include <X11/Xlib.h>
#include <Imlib2.h>

#ifdef USE_OPENMP
#include <omp.h>
#endif

typedef void* voidp;
typedef void** voidpp;
typedef unsigned char* ucharp;
typedef double mbnum;

float sqr(const float x);
int point_iteration(const mbnum cx, const mbnum cy, const int depth_max);

#endif /* _MANDELBROT_H */
