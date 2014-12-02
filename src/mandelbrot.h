
#ifndef _MANDELBROT_H
#define _MANDELBROT_H 1

#include "config.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include <X11/Xlib.h>
#include <Imlib2.h>
#include <glib.h>

#include <omp.h>

typedef void* voidp;
typedef void** voidpp;
typedef unsigned char* ucharp;

float sqr(const float x);
int point_iteration(const float cx, const float cy, const int depth_max);

#endif /* _MANDELBROT_H */
