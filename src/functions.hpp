
#ifndef _MANDELBROT_FUNCTIONS_H
#define _MANDELBROT_FUNCTIONS_H 1

#include "config.h"

#ifdef __cplusplus
#include <cmath>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


#include "types.h"

void print_copyright();
void print_config(int, int, int, int, int, int, int, int, float, mbnum, mbnum, mbnum, mbnum, mbnum, mbnum, mbnum, mbnum, mbnum, mbnum);
int point_iteration(const mbnum_t, const mbnum_t, const int);
void data_file_name(char *, int, int, int, int, mbnum_t, mbnum_t, mbnum_t, mbnum_t, int);

#endif
