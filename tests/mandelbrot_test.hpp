
#ifndef _MANDELBROT_TEST_H
#define _MANDELBROT_TEST_H 1

#include "../src/config.h"
#include "../src/functions.hpp"

#include <cmath>

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

#ifdef USE_OPENCL
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#endif

#endif
