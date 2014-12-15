
#ifndef _MANDELBROT_FUNCTIONS_H
#define _MANDELBROT_FUNCTIONS_H 1

#include "config.h"

#ifdef __cplusplus
#include <cmath>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <libgen.h>

#ifdef USE_OPENCL
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#endif

#include "types.h"

int point_iteration(const mbnum_t, const mbnum_t, const int);
void data_file_name(char *, int, int, int, int, mbnum_t, mbnum_t, mbnum_t, mbnum_t, int);

#ifdef USE_OPENCL
//void pfn_notify(const char *, const void *, size_t, void *);
void pfn_notify(cl_program, void *);
#endif

#endif
