
#ifndef _MANDELBROT_CI_H
#define _MANDELBROT_CI_H 1

#include "config.h"
#include "functions.hpp"

#include <cmath>
#include <fstream>
#include <string>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include <X11/Xlib.h>
#include <Imlib2.h>

#ifdef USE_OPENMP
#include <omp.h>
#endif

#undef USE_OPENMP
#undef USE_MB_XY_GRID

#endif
