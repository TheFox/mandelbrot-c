
#ifndef _MANDELBROT_H
#define _MANDELBROT_H

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <X11/Xlib.h>
#include <Imlib2.h>
#include <glib.h>

float sqr(const float x);
int point_iteration(const float cx, const float cy, const int depth_max);

#endif /* !_MANDELBROT_H */
