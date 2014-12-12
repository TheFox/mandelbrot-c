
#ifndef _MANDELBROT_H
#define _MANDELBROT_H 1

#include "config.h"
#include "functions.hpp"

//#include <iostream>
#include <cmath>
//#include <complex>

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

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#endif
