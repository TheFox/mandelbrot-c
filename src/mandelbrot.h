
#ifndef _MANDELBROT_H
#define _MANDELBROT_H

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <X11/Xlib.h>
#include <Imlib2.h>

int point_iteration(const float cx, const float cy);

#endif /* !_MANDELBROT_H */
